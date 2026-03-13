#pragma once

#include "CoreMinimal.h"
#include "WorldBossTypes.generated.h"

UENUM(BlueprintType)
enum class EWorldBossState : uint8
{
    Dormant,
    Spawned,
    Fighting,
    Dead
};

USTRUCT(BlueprintType)
struct FWorldBossData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 BossID = -1;

    UPROPERTY()
    FVector SpawnLocation;

    UPROPERTY()
    float MaxHealth = 0.f;

    UPROPERTY()
    float CurrentHealth = 0.f;

    UPROPERTY()
    EWorldBossState State = EWorldBossState::Dormant;

    UPROPERTY()
    FDateTime LastDeathTime;
};
