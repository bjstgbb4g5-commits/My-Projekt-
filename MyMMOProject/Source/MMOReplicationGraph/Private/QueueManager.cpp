#include "QueueManager.h"
#include "PlayerRegistrySubsystem.h"
#include "Engine/World.h"

bool UQueueManager::AddToQueue(EMatchQueueType QueueType, const FPlayerQueueData& Data)
{
    // Проверка, не в очереди ли уже
    EMatchQueueType Existing;
    if (IsPlayerInQueue(Data.PlayerId, Existing))
        return false;

    Queues.FindOrAdd(QueueType).Add(Data);
    PlayerQueueMap.Add(Data.PlayerId, QueueType);
    SortQueue(QueueType);
    return true;
}

bool UQueueManager::RemoveFromAllQueues(const FUniqueNetIdRepl& PlayerId)
{
    EMatchQueueType QueueType;
    if (!IsPlayerInQueue(PlayerId, QueueType))
        return false;

    TArray<FPlayerQueueData>& Queue = Queues[QueueType];
    Queue.RemoveAll([&](const FPlayerQueueData& D) { return D.PlayerId == PlayerId; });
    PlayerQueueMap.Remove(PlayerId);
    return true;
}

TArray<FPlayerQueueData> UQueueManager::GetQueue(EMatchQueueType QueueType) const
{
    const TArray<FPlayerQueueData>* Found = Queues.Find(QueueType);
    return Found ? *Found : TArray<FPlayerQueueData>();
}

TArray<FPlayerQueueData> UQueueManager::PopTopPlayers(EMatchQueueType QueueType, int32 Count)
{
    TArray<FPlayerQueueData> Result;
    TArray<FPlayerQueueData>* Queue = Queues.Find(QueueType);
    if (!Queue) return Result;

    if (Queue->Num() < Count) return Result;

    // Копируем первых Count
    Result.Append(Queue->GetData(), Count);
    // Удаляем их из очереди
    Queue->RemoveAt(0, Count);
    // Удаляем из карты
    for (const FPlayerQueueData& Data : Result)
    {
        PlayerQueueMap.Remove(Data.PlayerId);
    }
    return Result;
}

int32 UQueueManager::GetQueueSize(EMatchQueueType QueueType) const
{
    const TArray<FPlayerQueueData>* Queue = Queues.Find(QueueType);
    return Queue ? Queue->Num() : 0;
}

bool UQueueManager::IsPlayerInQueue(const FUniqueNetIdRepl& PlayerId, EMatchQueueType& OutQueueType) const
{
    const EMatchQueueType* Found = PlayerQueueMap.Find(PlayerId);
    if (Found)
    {
        OutQueueType = *Found;
        return true;
    }
    return false;
}

void UQueueManager::SortQueue(EMatchQueueType QueueType)
{
    TArray<FPlayerQueueData>* Queue = Queues.Find(QueueType);
    if (Queue)
    {
        Queue->Sort([](const FPlayerQueueData& A, const FPlayerQueueData& B) {
            return A < B;
        });
    }
}

void UQueueManager::CleanupDisconnectedPlayers()
{
    UWorld* World = GetWorld();
    if (!World) return;

    UPlayerRegistrySubsystem* Registry = World->GetSubsystem<UPlayerRegistrySubsystem>();
    if (!Registry) return;

    TArray<FUniqueNetIdRepl> ToRemove;
    for (const auto& Pair : PlayerQueueMap)
    {
        APlayerController* PC = Registry->GetPlayerById(Pair.Key);
        if (!PC || !PC->GetPawn())
        {
            ToRemove.Add(Pair.Key);
        }
    }

    for (const auto& PlayerId : ToRemove)
    {
        RemoveFromAllQueues(PlayerId);
    }
}
