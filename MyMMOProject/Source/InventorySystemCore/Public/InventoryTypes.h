#pragma once

#include "CoreMinimal.h"
#include "ItemSystemCore/Public/ItemTypes.h" // для EItemRarity
#include "InventoryTypes.generated.h"

UENUM(BlueprintType)
enum class EInventoryContainerType : uint8
{
    PlayerInventory,
    Equipment,
    Hotbar,
    Storage,
    GuildStorage
};

USTRUCT(BlueprintType)
struct FItemInstance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ItemID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Quantity = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxStack = 999;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Durability = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EItemRarity Rarity = EItemRarity::Gray;
};

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY()
    bool bOccupied = false;

    UPROPERTY()
    FItemInstance Item;
};

USTRUCT(BlueprintType)
struct FInventoryContainer
{
    GENERATED_BODY()

    UPROPERTY()
    EInventoryContainerType Type = EInventoryContainerType::PlayerInventory;

    UPROPERTY()
    int32 MaxSlots = 0;

    UPROPERTY()
    TArray<FInventorySlot> Slots;

    void InitContainer(int32 InMaxSlots, EInventoryContainerType InType)
    {
        Type = InType;
        MaxSlots = InMaxSlots;
        Slots.SetNum(MaxSlots);
    }
};
