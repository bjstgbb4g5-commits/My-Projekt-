#pragma once

#include "CoreMinimal.h"
#include "ApostleTypes.generated.h"

UENUM(BlueprintType)
enum class EApostleType : uint8
{
    War,
    Plague,
    Void,
    Storm,
    Flame,
    Nox
};

UENUM(BlueprintType)
enum class EApostleState : uint8
{
    Dormant,
    Active,
    Dead
};

USTRUCT(BlueprintType)
struct FApostleData
{
    GENERATED_BODY()

    UPROPERTY()
    EApostleType Apostle = EApostleType::War;

    UPROPERTY()
    FVector SpawnLocation;

    UPROPERTY()
    float MaxHealth = 0.f;

    UPROPERTY()
    float CurrentHealth = 0.f;

    UPROPERTY()
    bool bAlive = true;

    UPROPERTY()
    EApostleState State = EApostleState::Dormant;
};
