#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PopulationTypes.h"
#include "WorldGeneratorCore/Public/WorldGeneratorTypes.h"
#include "WorldPopulationSubsystem.generated.h"

class UWorldGeneratorSubsystem;
class UWildlifeSpawner;
class UBanditCampSpawner;
class UVillageSpawner;
class UWorldBossSpawner;
class UNPCPatrolSpawner;

UCLASS()
class WORLDPOPULATIONCORE_API UWorldPopulationSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UWorldPopulationSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "World Population")
    void PopulateWorld();

    // Доступ к данным о населении
    const TArray<FBanditCampData>& GetBanditCamps() const { return BanditCamps; }
    const TArray<FWorldBossSpawnData>& GetWorldBosses() const { return WorldBosses; }
    const TArray<FVillageSpawnData>& GetVillages() const { return Villages; }

protected:
    UPROPERTY()
    UWorldGeneratorSubsystem* WorldGen;

    UPROPERTY()
    UWildlifeSpawner* WildlifeSpawner;

    UPROPERTY()
    UBanditCampSpawner* BanditCampSpawner;

    UPROPERTY()
    UVillageSpawner* VillageSpawner;

    UPROPERTY()
    UWorldBossSpawner* WorldBossSpawner;

    UPROPERTY()
    UNPCPatrolSpawner* NPCPatrolSpawner;

    // Сгенерированные данные (могут быть сохранены/загружены)
    UPROPERTY()
    TArray<FBanditCampData> BanditCamps;

    UPROPERTY()
    TArray<FWorldBossSpawnData> WorldBosses;

    UPROPERTY()
    TArray<FVillageSpawnData> Villages;

    UPROPERTY()
    TArray<FNPCPatrolData> NPCPatrols;

private:
    void SpawnWildlife();
    void SpawnBanditCamps();
    void SpawnVillages();
    void SpawnWorldBosses();
    void SpawnNPCPatrols();
};
