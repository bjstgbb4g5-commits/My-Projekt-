#pragma once

#include "CoreMinimal.h"
#include "CombatAITypes.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
    Idle,
    Engaging,
    Fighting,
    Retreating,
    Dead
};

UENUM(BlueprintType)
enum class EOptimizationTier : uint8
{
    Full,
    Reduced,
    Minimal,
    Off
};
