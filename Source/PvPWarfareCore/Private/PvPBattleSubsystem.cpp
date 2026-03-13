#include "PvPBattleSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "CityPoliticsCore/Public/CityGovernanceComponent.h"
#include "WorldEconomyCore/Public/WorldEconomySubsystem.h"
#include "GuildSystemCore/Public/GuildSubsystem.h"
#include "Misc/DateTime.h"

void UPvPBattleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UPvPBattleSubsystem::Deinitialize()
{
    ActiveBattles.Empty();
    BattleLogs.Empty();
    LastBattleLeave.Empty();
    Super::Deinitialize();
}

bool UPvPBattleSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UPvPBattleSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UPvPBattleSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UPvPBattleSubsystem, STATGROUP_Tickables);
}

void UPvPBattleSubsystem::Tick(float DeltaTime)
{
    if (!IsServer()) return;

    TArray<int32> BattlesToFinish;

    for (auto& Pair : ActiveBattles)
    {
        FPvPBattle& Battle = Pair.Value;
        if (Battle.State == EPvPBattleState::Active)
        {
            Battle.TimeRemaining -= DeltaTime;
            if (Battle.TimeRemaining <= 0.f)
            {
                BattlesToFinish.Add(Battle.BattleID);
            }
        }
    }

    for (int32 BattleID : BattlesToFinish)
    {
        FinishBattle(BattleID, EPvPBattleResult::Timeout);
    }
}

int32 UPvPBattleSubsystem::CreateBattle(EPvPBattleType Type, int32 MaxAttackers, int32 MaxDefenders, float DurationSeconds)
{
    if (!IsServer()) return -1;

    FPvPBattle NewBattle;
    NewBattle.BattleID = NextBattleID++;
    NewBattle.BattleType = Type;
    NewBattle.State = EPvPBattleState::Waiting;
    NewBattle.MaxAttackers = MaxAttackers;
    NewBattle.MaxDefenders = MaxDefenders;
    NewBattle.BattleDuration = DurationSeconds;
    NewBattle.TimeRemaining = DurationSeconds;
    ActiveBattles.Add(NewBattle.BattleID, NewBattle);
    return NewBattle.BattleID;
}

bool UPvPBattleSubsystem::JoinBattle(int32 BattleID, APlayerController* Player, bool bAsAttacker)
{
    if (!IsServer()) return false;
    if (!Player) return false;

    APlayerState* PS = Player->GetPlayerState<APlayerState>();
    if (!PS) return false;

    // Проверка cooldown
    if (float* LastLeave = LastBattleLeave.Find(PS))
    {
        float Now = GetWorld()->GetTimeSeconds();
        if (Now - *LastLeave < REJOIN_COOLDOWN)
        {
            return false;
        }
    }

    FPvPBattle* Battle = ActiveBattles.Find(BattleID);
    if (!Battle) return false;

    if (Battle->State != EPvPBattleState::Waiting &&
        Battle->State != EPvPBattleState::Preparing)
        return false;

    // Уже в бою?
    if (Battle->Attackers.Contains(PS) || Battle->Defenders.Contains(PS))
        return false;

    TArray<APlayerState*>& Team = bAsAttacker ? Battle->Attackers : Battle->Defenders;
    int32 Max = bAsAttacker ? Battle->MaxAttackers : Battle->MaxDefenders;

    if (Team.Num() >= Max)
        return false;

    Team.Add(PS);
    return true;
}

void UPvPBattleSubsystem::StartBattle(int32 BattleID)
{
    if (!IsServer()) return;

    FPvPBattle* Battle = ActiveBattles.Find(BattleID);
    if (!Battle) return;

    if (Battle->Attackers.Num() == 0 || Battle->Defenders.Num() == 0)
        return;

    Battle->State = EPvPBattleState::Active;
    Battle->StartTime = FDateTime::UtcNow();
    OnBattleStarted.Broadcast(BattleID);
}

void UPvPBattleSubsystem::FinishBattle(int32 BattleID, EPvPBattleResult Result)
{
    if (!IsServer()) return;

    FPvPBattle* Battle = ActiveBattles.Find(BattleID);
    if (!Battle) return;

    Battle->State = EPvPBattleState::Finished;

    // Логируем
    FPvPBattleLog Log;
    Log.BattleID = BattleID;
    Log.Type = Battle->BattleType;
    Log.AttackersCount = Battle->Attackers.Num();
    Log.DefendersCount = Battle->Defenders.Num();
    Log.Duration = Battle->BattleDuration - Battle->TimeRemaining;
    Log.Result = Result;

    if (BattleLogs.Num() >= MAX_LOGS)
    {
        BattleLogs.RemoveAt(0);
    }
    BattleLogs.Add(Log);

    // Обновляем время выхода для всех участников
    for (APlayerState* PS : Battle->Attackers)
    {
        LastBattleLeave.Add(PS, GetWorld()->GetTimeSeconds());
    }
    for (APlayerState* PS : Battle->Defenders)
    {
        LastBattleLeave.Add(PS, GetWorld()->GetTimeSeconds());
    }

    // Интеграция с экономикой/политикой (заглушки)
    UWorld* World = GetWorld();
    if (World)
    {
        UWorldEconomySubsystem* Economy = World->GetSubsystem<UWorldEconomySubsystem>();
        if (Economy) Economy->UpdateTerritoryControl(0); // заглушка

        UGuildSubsystem* Guilds = World->GetSubsystem<UGuildSubsystem>();
        if (Guilds) Guilds->AddGuildReputation(0, 100); // заглушка
    }

    OnBattleFinished.Broadcast(BattleID, Result);
    ActiveBattles.Remove(BattleID);
}

FPvPBattle UPvPBattleSubsystem::GetBattleInfo(int32 BattleID) const
{
    const FPvPBattle* Battle = ActiveBattles.Find(BattleID);
    return Battle ? *Battle : FPvPBattle();
}
