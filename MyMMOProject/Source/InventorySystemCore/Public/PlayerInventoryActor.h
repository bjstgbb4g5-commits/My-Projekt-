#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryTypes.h"
#include "HotbarTypes.h"
#include "EquipmentSlots.h"
#include "PlayerInventoryActor.generated.h"

UCLASS()
class INVENTORYSYSTEMCORE_API APlayerInventoryActor : public AActor
{
    GENERATED_BODY()

public:
    APlayerInventoryActor();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Основные контейнеры
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
    FInventoryContainer Inventory;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
    FInventoryContainer Equipment;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
    TArray<FHotbarSlot> HotbarSlots; // вместо отдельного контейнера

    // Вспомогательные методы
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool IsValidSlot(EInventoryContainerType ContainerType, int32 SlotIndex) const;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    FInventorySlot* GetSlot(EInventoryContainerType ContainerType, int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    const FInventorySlot* GetSlotConst(EInventoryContainerType ContainerType, int32 SlotIndex) const;

    // Работа с хотбаром
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool SetHotbarSlot(int32 HotbarIndex, int32 InventorySlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 GetInventorySlotFromHotbar(int32 HotbarIndex) const;

    // Очередь крафта (для интеграции с CraftingSystem)
    UPROPERTY(Replicated, BlueprintReadOnly)
    TArray<struct FCraftQueueItem> CraftQueue;

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    int32 GenerateNextQueueID();

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void AddToCraftQueue(int32 QueueID, int32 RecipeID, float StartTime, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void RemoveFromCraftQueue(int32 QueueID);

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    const TArray<FCraftQueueItem>& GetCraftQueue() const { return CraftQueue; }

private:
    int32 NextQueueID = 1;
    bool IsServer() const;
};
