#include "DungeonSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UDungeonSubsystem::UDungeonSubsystem()
{
}

void UDungeonSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        GetWorld()->GetTimerManager().SetTimer(CleanupTimer, this, &UDungeonSubsystem::CleanupInstances, CLEANUP_INTERVAL, true);
    }
}

void UDungeonSubsystem::Deinitialize()
{
    if (IsServer())
    {
        GetWorld()->GetTimerManager().ClearTimer(CleanupTimer);
    }
    ActiveDungeons.Empty();
    DungeonAttempts.Empty();
    Super::Deinitialize();
}

bool UDungeonSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

int32 UDungeonSubsystem::CreateDungeonInstance(const TArray<APlayerState*>& Players, EDungeonDifficulty Difficulty)
{
    if (!IsServer()) return -1;

    FDungeonInstance Instance;
    Instance.InstanceID = NextInstanceID++;
    Instance.Players = Players;
    Instance.Difficulty = Difficulty;
    Instance.State = EDungeonState::Waiting;
    Instance.bBossAlive = true;
    ActiveDungeons.Add(Instance.InstanceID, Instance);
    return Instance.InstanceID;
}

void UDungeonSubsystem::StartDungeon(int32 InstanceID)
{
    if (!IsServer()) return;
    FDungeonInstance* Instance = ActiveDungeons.Find(InstanceID);
    if (!Instance) return;
    Instance->State = EDungeonState::Active;
}

void UDungeonSubsystem::CompleteDungeon(int32 InstanceID)
{
    if (!IsServer()) return;
    FDungeonInstance* Instance = ActiveDungeons.Find(InstanceID);
    if (!Instance) return;
    Instance->State = EDungeonState::Completed;

    // Увеличиваем счётчик попыток
    for (APlayerState* PS : Instance->Players)
    {
        if (PS)
        {
            DungeonAttempts.FindOrAdd(PS).FindOrAdd(Instance->Difficulty)++;
        }
    }
}

bool UDungeonSubsystem::CanEnterDungeon(APlayerState* Player, EDungeonDifficulty Difficulty) const
{
    const TMap<EDungeonDifficulty, int32>* Attempts = DungeonAttempts.Find(Player);
    if (!Attempts) return true;

    int32 Count = Attempts->FindRef(Difficulty);
    if (Difficulty == EDungeonDifficulty::Mythic)
        return Count < 1;
    else if (Difficulty == EDungeonDifficulty::Elite)
        return Count < 3;
    else
        return Count < 5;
}

void UDungeonSubsystem::CleanupInstances()
{
    if (!IsServer()) return;
    TArray<int32> ToRemove;
    for (auto& Pair : ActiveDungeons)
    {
        if (Pair.Value.State == EDungeonState::Completed || Pair.Value.State == EDungeonState::Failed)
        {
            ToRemove.Add(Pair.Key);
        }
    }
    for (int32 ID : ToRemove)
    {
        ActiveDungeons.Remove(ID);
    }
}
