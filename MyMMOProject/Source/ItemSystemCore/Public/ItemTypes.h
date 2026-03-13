#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Resource,
    Weapon,
    Armor,
    Consumable,
    Artifact,
    Quest
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Sword,
    Axe,
    Bow,
    Staff
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
    Cloth,
    Leather,
    Plate
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    Gray,
    Green,
    Blue,
    Red,
    Purple,
    Orange
};
