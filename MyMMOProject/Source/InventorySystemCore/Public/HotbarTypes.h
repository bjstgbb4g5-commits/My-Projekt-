#pragma once

#include "CoreMinimal.h"
#include "HotbarTypes.generated.h"

USTRUCT(BlueprintType)
struct FHotbarSlot
{
    GENERATED_BODY()

    UPROPERTY()
    bool bIsEmpty = true;

    UPROPERTY()
    int32 InventorySlotIndex = -1; // ссылка на слот в основном инвентаре
};
