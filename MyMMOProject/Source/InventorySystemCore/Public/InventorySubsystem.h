#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "InventoryTypes.h"
#include "InventorySubsystem.generated.h"

class APlayerInventoryActor;

UCLASS()
class INVENTORYSYSTEMCORE_API UInventorySubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UInventorySubsystem();

    // Добавить предмет в первый свободный слот указанного контейнера
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(APlayerInventoryActor* InventoryActor, EInventoryContainerType ContainerType, const FItemInstance& Item);

    // Удалить предмет из слота (пометить слот как пустой)
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(APlayerInventoryActor* InventoryActor, EInventoryContainerType ContainerType, int32 SlotIndex);

    // Переместить предмет из одного слота в другой (в пределах одного контейнера)
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool MoveItem(APlayerInventoryActor* InventoryActor, EInventoryContainerType ContainerType, int32 FromSlot, int32 ToSlot);

    // Экипировать предмет из инвентаря в экипировку
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool EquipItem(APlayerInventoryActor* InventoryActor, int32 InventorySlot, int32 EquipmentSlot);

    // Снять предмет из экипировки в инвентарь
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool UnequipItem(APlayerInventoryActor* InventoryActor, int32 EquipmentSlot, int32 InventorySlot);

    // Подсчёт количества определённого предмета в инвентаре
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 CountItem(APlayerInventoryActor* InventoryActor, int32 ItemID) const;

    // Удалить указанное количество предмета (находит стопки)
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItemByID(APlayerInventoryActor* InventoryActor, int32 ItemID, int32 Amount);

private:
    bool IsServer() const;
    int32 CountItemInContainer(const FInventoryContainer& Container, int32 ItemID) const;
    bool RemoveItemByIDFromContainer(FInventoryContainer& Container, int32 ItemID, int32& Amount);
};
