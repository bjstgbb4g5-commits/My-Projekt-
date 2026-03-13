#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LootTypes.h"
#include "LootSubsystem.generated.h"

class ALootActor;
class APlayerInventoryActor;
class UAIDirectorSubsystem;

UCLASS()
class LOOTSYSTEMCORE_API ULootSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    ULootSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Генерирует список предметов из таблицы
    UFUNCTION(BlueprintCallable, Category = "Loot")
    TArray<FItemInstance> GenerateLoot(int32 LootTableID, ELootSourceType SourceType);

    // Спавнит лут в мире
    UFUNCTION(BlueprintCallable, Category = "Loot")
    void SpawnLoot(const FVector& Location, const TArray<FItemInstance>& Items);

    // Попытка подобрать лут игроком (возвращает true, если предмет добавлен в инвентарь)
    UFUNCTION(BlueprintCallable, Category = "Loot")
    bool PickupLoot(ALootActor* Loot, APlayerInventoryActor* Inventory);

    // Очистка массива ActiveLoot от уничтоженных объектов
    UFUNCTION(BlueprintCallable, Category = "Loot")
    void CleanupLootArray();

    static constexpr int32 MAX_LOOT_ACTORS = 500;

private:
    bool IsServer() const;
    void LoadLootTables();

    UPROPERTY()
    TMap<int32, FLootTable> LootTables;

    UPROPERTY()
    UAIDirectorSubsystem* AIDirector;

    UPROPERTY()
    class UItemSubsystem* ItemSystem; // для создания предметов

    TArray<TWeakObjectPtr<ALootActor>> ActiveLoot;
};
