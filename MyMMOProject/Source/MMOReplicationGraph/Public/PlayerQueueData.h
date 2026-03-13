#pragma once

#include "CoreMinimal.h"
#include "QueueTypes.h"
#include "PlayerQueueData.generated.h"

USTRUCT()
struct FPlayerQueueData
{
    GENERATED_BODY()

    UPROPERTY()
    FUniqueNetIdRepl PlayerId;

    UPROPERTY()
    int32 Level = 0;

    UPROPERTY()
    int32 GearScore = 0;

    UPROPERTY()
    EPlayerRole Role = EPlayerRole::DPS;

    UPROPERTY()
    FDateTime QueueJoinTime;

    // Функция сравнения для сортировки (по геарскор, потом по времени)
    bool operator<(const FPlayerQueueData& Other) const
    {
        if (GearScore != Other.GearScore)
            return GearScore > Other.GearScore; // выше геарскор – выше приоритет
        return QueueJoinTime < Other.QueueJoinTime;
    }
};
