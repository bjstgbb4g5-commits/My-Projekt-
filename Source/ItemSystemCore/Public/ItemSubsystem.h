#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ItemDefinition.h"
#include "InventoryTypes.h" // для FItemInstance
#include "ItemSubsystem.generated.h"

UCLASS()
class ITEMSYSTEMCORE_API UItemSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UItemSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Получить определение предмета по ID
    UFUNCTION(BlueprintCallable, Category = "Item")
    const FItemDefinition* GetItemDefinition(int32 ItemID) const;

    // Создать экземпляр предмета (используется Loot, Crafting и т.д.)
    UFUNCTION(BlueprintCallable, Category = "Item")
    FItemInstance CreateItem(int32 ItemID, int32 Quantity);

private:
    void LoadItems();

    UPROPERTY()
    TMap<int32, FItemDefinition> ItemDefinitions;
};
