#include "RaidSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerState.h"

URaidSubsystem::URaidSubsystem()
{
}

void URaidSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        GetWorld()->GetTimerManager().SetTimer(CleanupTimer, this, &URaidSubsystem::CleanupCompletedRaids, CLEANUP_INTERVAL, true);
    }
}

void URaidSubsystem::Deinitialize()
{
    if (IsServer())
    {
        GetWorld()->GetTimerManager().ClearTimer(CleanupTimer);
    }
    ActiveRaids.Empty();
    Super::Deinitialize();
}

bool URaidSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

int32 URaidSubsystem::CreateRaid(const TArray<APlayerState*>& Players, ERaidDifficulty Difficulty)
{
    if (!IsServer()) return -1;
    if (Players.Num() == 0) return -1;

    FRaidInstance Raid;
    Raid.RaidID = NextRaidID++;
    Raid.Players = Players;
    Raid.Difficulty = Difficulty;
    Raid.State = ERaidState::Waiting;
    Raid.BossIndex = 0;
    Raid.BossCount = 3; // можно загружать из конфига
    ActiveRaids.Add(Raid.RaidID, Raid);
    return Raid.RaidID;
}

void URaidSubsystem::StartRaid(int32 RaidID)
{
    if (!IsServer()) return;
    FRaidInstance* Raid = ActiveRaids.Find(RaidID);
    if (!Raid) return;
    if (Raid->State != ERaidState::Waiting) return;
    Raid->State = ERaidState::Active;
}

void URaidSubsystem::BossKilled(int32 RaidID)
{
    if (!IsServer()) return;
    FRaidInstance* Raid = ActiveRaids.Find(RaidID);
    if (!Raid) return;
    if (Raid->State != ERaidState::Active && Raid->State != ERaidState::BossFight) return;

    Raid->BossIndex++;
    if (Raid->BossIndex >= Raid->BossCount)
    {
        Raid->State = ERaidState::Completed;
        // Можно вызвать событие завершения рейда, выдать награды
    }
    else
    {
        Raid->State = ERaidState::BossFight; // или остаётся Active
    }
}

FRaidInstance URaidSubsystem::GetRaidInfo(int32 RaidID) const
{
    const FRaidInstance* Raid = ActiveRaids.Find(RaidID);
    return Raid ? *Raid : FRaidInstance();
}

TArray<int32> URaidSubsystem::GetActiveRaidIDs() const
{
    TArray<int32> Keys;
    ActiveRaids.GetKeys(Keys);
    return Keys;
}

void URaidSubsystem::CleanupCompletedRaids()
{
    if (!IsServer()) return;
    TArray<int32> ToRemove;
    for (const auto& Pair : ActiveRaids)
    {
        if (Pair.Value.State == ERaidState::Completed || Pair.Value.State == ERaidState::Failed)
        {
            ToRemove.Add(Pair.Key);
        }
    }
    for (int32 ID : ToRemove)
    {
        ActiveRaids.Remove(ID);
    }
}
