#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "InventoryTypes.h"
#include "InventorySaveGame.generated.h"

UCLASS()
class INVENTORYSYSTEMCORE_API UInventorySaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    FInventoryContainer Inventory;

    UPROPERTY()
    FInventoryContainer Equipment;

    UPROPERTY()
    TArray<FHotbarSlot> HotbarSlots;
};
