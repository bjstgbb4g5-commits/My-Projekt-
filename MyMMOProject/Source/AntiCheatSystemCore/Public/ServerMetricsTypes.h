#pragma once

#include "CoreMinimal.h"
#include "ServerMetricsTypes.generated.h"

USTRUCT()
struct FPlayerMetrics
{
    GENERATED_BODY()

    UPROPERTY()
    FUniqueNetIdRepl PlayerID;

    UPROPERTY()
    int32 DamagePerMinute = 0;

    UPROPERTY()
    int32 GoldPerMinute = 0;

    UPROPERTY()
    int32 KillsPerMinute = 0;

    UPROPERTY()
    float MovementSpeedAverage = 0.f;

    UPROPERTY()
    int32 ActionsPerMinute = 0;

    UPROPERTY()
    FDateTime LastUpdate;
};
