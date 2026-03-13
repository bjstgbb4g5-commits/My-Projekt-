#pragma once

#include "CoreMinimal.h"
#include "QueueTypes.h"
#include "PlayerQueueData.h"
#include "QueueManager.generated.h"

UCLASS()
class MATCHMAKINGSYSTEMCORE_API UQueueManager : public UObject
{
    GENERATED_BODY()

public:
    // Добавить игрока в очередь
    bool AddToQueue(EMatchQueueType QueueType, const FPlayerQueueData& Data);

    // Удалить игрока из всех очередей
    bool RemoveFromAllQueues(const FUniqueNetIdRepl& PlayerId);

    // Получить копию очереди
    TArray<FPlayerQueueData> GetQueue(EMatchQueueType QueueType) const;

    // Извлечь лучших игроков для создания матча (с удалением)
    TArray<FPlayerQueueData> PopTopPlayers(EMatchQueueType QueueType, int32 Count);

    // Получить размер очереди
    int32 GetQueueSize(EMatchQueueType QueueType) const;

    // Проверить, находится ли игрок в любой очереди
    bool IsPlayerInQueue(const FUniqueNetIdRepl& PlayerId, EMatchQueueType& OutQueueType) const;

    // Очистка отключившихся игроков
    void CleanupDisconnectedPlayers();

private:
    TMap<EMatchQueueType, TArray<FPlayerQueueData>> Queues;
    TMap<FUniqueNetIdRepl, EMatchQueueType> PlayerQueueMap; // быстрый поиск

    // Сортировка очереди (вызывать после добавления)
    void SortQueue(EMatchQueueType QueueType);
};
