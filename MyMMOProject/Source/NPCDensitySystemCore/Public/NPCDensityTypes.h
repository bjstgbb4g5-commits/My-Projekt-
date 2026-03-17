#pragma once

#include "CoreMinimal.h"
#include "NPCDensityTypes.generated.h"

UENUM(BlueprintType)
enum class ENPCLODLevel : uint8
{
    LOD0 = 0,
    LOD1 = 1,
    LOD2 = 2,
    LOD3 = 3
};

USTRUCT()
struct FNPCPoolEntry
{
    GENERATED_BODY()

    TWeakObjectPtr<AActor> NPCActor;
    bool bActive = false;
};

USTRUCT(BlueprintType)
struct FNPCDensityZoneConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    int32 TargetNPCCount = 10;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> NPCClass;

    UPROPERTY(EditAnywhere)
    float SpawnRadius = 1000.0f;

    UPROPERTY(EditAnywhere)
    int32 MaxNPCInZone = 50;

    UPROPERTY(EditAnywhere)
    int32 PoolSize = 100;
};
