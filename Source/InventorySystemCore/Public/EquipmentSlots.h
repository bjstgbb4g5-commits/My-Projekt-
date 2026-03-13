#pragma once

#include "CoreMinimal.h"
#include "EquipmentSlots.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
    Head,
    Chest,
    Legs,
    Weapon,
    Offhand,
    Ring1,
    Ring2,
    Amulet
};
