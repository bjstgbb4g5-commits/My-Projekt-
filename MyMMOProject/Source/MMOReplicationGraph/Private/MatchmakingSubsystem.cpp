#include "MatchmakingSubsystem.h"
#include "QueueManager.h"
#include "MatchmakingComponent.h"
#include "PlayerRegistrySubsystem.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Misc/DateTime.h"

UMatchmakingSubsystem::UMatchmakingSubsystem()
{
}

void UMatchmakingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        QueueManager = NewObject<UQueueManager>(this);
    }
}

void UMatchmakingSubsystem::Deinitialize()
{
    ActiveMatches.Empty();
    QueueManager = nullptr;
    Super::Deinitialize();
}

bool UMatchmakingSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UMatchmakingSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UMatchmakingSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UMatchmakingSubsystem, STATGROUP_Tickables);
}

void UMatchmakingSubsystem::Tick(float DeltaTime)
{
    if (!QueueManager) return;

    // Очищаем отключившихся
    QueueManager->CleanupDisconnectedPlayers();

    // Проверяем очереди
    ProcessArena1v1();
    ProcessArena3v3();
    ProcessBattleground();
    ProcessDungeon();
    ProcessRaid();
}

UMatchmakingComponent* UMatchmakingSubsystem::GetMatchmakingComponent(APlayerController* Player) const
{
    if (!Player || !Player->GetPawn()) return nullptr;
    return Player->GetPawn()->FindComponentByClass<UMatchmakingComponent>();
}

bool UMatchmakingSubsystem::JoinQueue(APlayerController* Player, EMatchQueueType QueueType, EPlayerRole Role)
{
    if (!IsServer() || !Player) return false;

    APlayerState* PS = Player->PlayerState;
    if (!PS) return false;

    // Получаем данные игрока (уровень, гирскор) из компонентов
    // В реальном проекте нужно взять из PlayerProgressionComponent и т.д.
    // Для примера используем заглушки
    int32 Level = 1; // TODO: из прогрессии
    int32 GearScore = 0; // TODO: из снаряжения

    FPlayerQueueData Data;
    Data.PlayerId = PS->GetUniqueId();
    Data.Level = Level;
    Data.GearScore = GearScore;
    Data.Role = Role;
    Data.QueueJoinTime = FDateTime::UtcNow();

    if (!QueueManager->AddToQueue(QueueType, Data))
        return false;

    // Обновить компонент игрока
    if (UMatchmakingComponent* Comp = GetMatchmakingComponent(Player))
    {
        Comp->SetQueueState(true, QueueType);
    }

    return true;
}

bool UMatchmakingSubsystem::LeaveQueue(APlayerController* Player)
{
    if (!IsServer() || !Player) return false;
    APlayerState* PS = Player->PlayerState;
    if (!PS) return false;

    bool bRemoved = QueueManager->RemoveFromAllQueues(PS->GetUniqueId());
    if (bRemoved)
    {
        if (UMatchmakingComponent* Comp = GetMatchmakingComponent(Player))
        {
            Comp->SetQueueState(false, EMatchQueueType::Arena1v1);
        }
    }
    return bRemoved;
}

void UMatchmakingSubsystem::NotifyMatchFound(const FMatchInstance& Match)
{
    UPlayerRegistrySubsystem* Registry = GetWorld()->GetSubsystem<UPlayerRegistrySubsystem>();
    if (!Registry) return;

    for (const FUniqueNetIdRepl& PlayerId : Match.Players)
    {
        APlayerController* PC = Registry->GetPlayerById(PlayerId);
        if (!PC) continue;

        if (UMatchmakingComponent* Comp = GetMatchmakingComponent(PC))
        {
            Comp->Client_MatchFound(Match.MatchId, Match.QueueType, Match.Players);
            Comp->SetQueueState(false, EMatchQueueType::Arena1v1);
        }
    }
}

void UMatchmakingSubsystem::ProcessArena1v1()
{
    const int32 RequiredPlayers = 2;
    while (QueueManager->GetQueueSize(EMatchQueueType::Arena1v1) >= RequiredPlayers)
    {
        TArray<FPlayerQueueData> Players = QueueManager->PopTopPlayers(EMatchQueueType::Arena1v1, RequiredPlayers);
        if (Players.Num() < RequiredPlayers) break;

        FMatchInstance Match;
        Match.MatchId = NextMatchId++;
        Match.QueueType = EMatchQueueType::Arena1v1;
        Match.StartTime = FDateTime::UtcNow();
        for (const FPlayerQueueData& Data : Players)
        {
            Match.Players.Add(Data.PlayerId);
        }
        ActiveMatches.Add(Match);
        NotifyMatchFound(Match);
    }
}

void UMatchmakingSubsystem::ProcessArena3v3()
{
    const int32 RequiredPlayers = 6;
    while (QueueManager->GetQueueSize(EMatchQueueType::Arena3v3) >= RequiredPlayers)
    {
        TArray<FPlayerQueueData> Players = QueueManager->PopTopPlayers(EMatchQueueType::Arena3v3, RequiredPlayers);
        if (Players.Num() < RequiredPlayers) break;

        FMatchInstance Match;
        Match.MatchId = NextMatchId++;
        Match.QueueType = EMatchQueueType::Arena3v3;
        Match.StartTime = FDateTime::UtcNow();
        for (const FPlayerQueueData& Data : Players)
        {
            Match.Players.Add(Data.PlayerId);
        }
        ActiveMatches.Add(Match);
        NotifyMatchFound(Match);
    }
}

void UMatchmakingSubsystem::ProcessBattleground()
{
    const int32 RequiredPlayers = 20;
    while (QueueManager->GetQueueSize(EMatchQueueType::Battleground) >= RequiredPlayers)
    {
        TArray<FPlayerQueueData> Players = QueueManager->PopTopPlayers(EMatchQueueType::Battleground, RequiredPlayers);
        if (Players.Num() < RequiredPlayers) break;

        FMatchInstance Match;
        Match.MatchId = NextMatchId++;
        Match.QueueType = EMatchQueueType::Battleground;
        Match.StartTime = FDateTime::UtcNow();
        for (const FPlayerQueueData& Data : Players)
        {
            Match.Players.Add(Data.PlayerId);
        }
        ActiveMatches.Add(Match);
        NotifyMatchFound(Match);
    }
}

bool UMatchmakingSubsystem::CheckDungeonRoles(const TArray<FPlayerQueueData>& Players) const
{
    int32 Tanks = 0, Healers = 0, DPS = 0;
    for (const FPlayerQueueData& P : Players)
    {
        switch (P.Role)
        {
        case EPlayerRole::Tank: Tanks++; break;
        case EPlayerRole::Healer: Healers++; break;
        case EPlayerRole::DPS: DPS++; break;
        default: DPS++; break;
        }
    }
    return Tanks >= 1 && Healers >= 1 && DPS >= 3;
}

void UMatchmakingSubsystem::ProcessDungeon()
{
    const int32 RequiredPlayers = 5;
    TArray<FPlayerQueueData> Queue = QueueManager->GetQueue(EMatchQueueType::Dungeon);
    if (Queue.Num() < RequiredPlayers) return;

    // Пытаемся составить группу с нужными ролями
    // Простой алгоритм: сортируем по времени и геарскору, затем проходим и набираем
    TArray<FPlayerQueueData> Selected;
    int32 Tanks = 0, Healers = 0, DPS = 0;
    TArray<int32> IndicesToRemove;

    for (int32 i = 0; i < Queue.Num() && Selected.Num() < RequiredPlayers; ++i)
    {
        const FPlayerQueueData& P = Queue[i];
        bool bAdd = false;
        if (P.Role == EPlayerRole::Tank && Tanks < 1)
        {
            Tanks++; bAdd = true;
        }
        else if (P.Role == EPlayerRole::Healer && Healers < 1)
        {
            Healers++; bAdd = true;
        }
        else if (P.Role == EPlayerRole::DPS && DPS < 3)
        {
            DPS++; bAdd = true;
        }
        else if (P.Role == EPlayerRole::Any)
        {
            // Если не хватает какой-то роли, можно назначить, но упростим
            if (Tanks < 1) { Tanks++; bAdd = true; }
            else if (Healers < 1) { Healers++; bAdd = true; }
            else if (DPS < 3) { DPS++; bAdd = true; }
        }

        if (bAdd)
        {
            Selected.Add(P);
            IndicesToRemove.Add(i);
        }
    }

    if (Selected.Num() == RequiredPlayers && CheckDungeonRoles(Selected))
    {
        // Удаляем выбранных из очереди
        for (int32 i = IndicesToRemove.Num() - 1; i >= 0; --i)
        {
            const FPlayerQueueData& P = Selected[i];
            QueueManager->RemoveFromAllQueues(P.PlayerId);
        }

        FMatchInstance Match;
        Match.MatchId = NextMatchId++;
        Match.QueueType = EMatchQueueType::Dungeon;
        Match.StartTime = FDateTime::UtcNow();
        for (const FPlayerQueueData& Data : Selected)
        {
            Match.Players.Add(Data.PlayerId);
        }
        ActiveMatches.Add(Match);
        NotifyMatchFound(Match);
    }
}

void UMatchmakingSubsystem::ProcessRaid()
{
    const int32 RequiredPlayers = 10;
    // Аналогично данжу, но без ролей (можно добавить позже)
    while (QueueManager->GetQueueSize(EMatchQueueType::Raid) >= RequiredPlayers)
    {
        TArray<FPlayerQueueData> Players = QueueManager->PopTopPlayers(EMatchQueueType::Raid, RequiredPlayers);
        if (Players.Num() < RequiredPlayers) break;

        FMatchInstance Match;
        Match.MatchId = NextMatchId++;
        Match.QueueType = EMatchQueueType::Raid;
        Match.StartTime = FDateTime::UtcNow();
        for (const FPlayerQueueData& Data : Players)
        {
            Match.Players.Add(Data.PlayerId);
        }
        ActiveMatches.Add(Match);
        NotifyMatchFound(Match);
    }
}
