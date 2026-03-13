#pragma once

#include "CoreMinimal.h"
#include "PerformanceTypes.generated.h"

UENUM(BlueprintType)
enum class EAILODLevel : uint8
{
    Active,
    Reduced,
    Sleeping
};

USTRUCT()
struct FRegionRuntimeState
{
    GENERATED_BODY()

    bool bIsLoaded = false;
    TArray<AActor*> SpawnedActors;
};

USTRUCT()
struct FServerMetrics
{
    GENERATED_BODY()

    int32 ActivePlayers = 0;
    int32 ActiveAI = 0;
    int32 LoadedRegions = 0;
    float ServerTickTime = 0.f;
    float AverageTickTime = 0.f;
};
