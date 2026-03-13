#pragma once

#include "CoreMinimal.h"
#include "MountTypes.generated.h"

UENUM(BlueprintType)
enum class EMountType : uint8
{
    Travel,
    Combat,
    Cargo
};

UENUM(BlueprintType)
enum class EMountSpecies : uint8
{
    Horse,
    Lizard,
    Bear,
    Deer,
    Mammoth
};

UENUM(BlueprintType)
enum class EMountRarity : uint8
{
    Common,
    Rare,
    Epic,
    Legendary,
    Mythic
};
