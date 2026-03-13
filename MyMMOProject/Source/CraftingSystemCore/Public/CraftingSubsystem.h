#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CraftingTypes.h"
#include "Engine/DataTable.h"
#include "CraftingSubsystem.generated.h"

class ACraftingStationActor;
class APlayerController;
class APlayerInventoryActor;
class UItemSubsystem;

UCLASS()
class CRAFTINGSYSTEMCORE_API UCraftingSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UCraftingSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Проверка возможности крафта
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    bool CanCraft(APlayerInventoryActor* Inventory, const FCraftingRecipe& Recipe) const;

    // Запуск крафта (помещает в очередь)
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    bool StartCraft(APlayerController* Player, int32 RecipeID);

    // Завершение крафта (вызывается по таймеру)
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void FinishCraft(APlayerInventoryActor* Inventory, int32 RecipeID);

    // Получение рецепта по ID
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    const FCraftingRecipe* GetRecipe(int32 RecipeID) const;

    // Получение всех рецептов для станции
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    TArray<FCraftingRecipe> GetRecipesForStation(ECraftingStationType StationType) const;

    // Генерация редкости предмета
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    EItemRarity GenerateRarity(const FCraftingRecipe& Recipe) const;

    // Проверка нахождения игрока рядом со станцией
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    bool IsPlayerNearStation(APlayerController* Player, ECraftingStationType StationType) const;

private:
    bool IsServer() const;
    void LoadRecipes();

    UPROPERTY()
    UDataTable* RecipeDataTable;

    UPROPERTY()
    TMap<int32, FCraftingRecipe> RecipeMap;

    // Кеш станций для быстрой проверки
    UPROPERTY()
    TMap<ECraftingStationType, TArray<ACraftingStationActor*>> StationCache;

    void CacheStations();
    void ClearStationCache();

    UPROPERTY()
    UItemSubsystem* ItemSystem; // для создания предметов
};
