#pragma once

#include "CoreMinimal.h"
#include "QueueTypes.h"
#include "MatchInstance.generated.h"

USTRUCT()
struct FMatchInstance
{
    GENERATED_BODY()

    UPROPERTY()
    int32 MatchId = -1;

    UPROPERTY()
    EMatchQueueType QueueType;

    UPROPERTY()
    TArray<FUniqueNetIdRepl> Players;

    UPROPERTY()
    FDateTime StartTime;
};
