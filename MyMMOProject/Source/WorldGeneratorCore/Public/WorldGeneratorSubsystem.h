#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldGeneratorTypes.h"
#include "Containers/Ticker.h"
#include "WorldGeneratorSubsystem.generated.h"

UCLASS()
class WORLDGENERATORCORE_API UWorldGeneratorSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UWorldGeneratorSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "World Generator")
    void GenerateWorld();

    UFUNCTION(BlueprintCallable, Category = "World Generator")
    const FWorldRegion* GetRegionAtLocation(FVector Location) const;

    UFUNCTION(BlueprintCallable, Category = "World Generator")
    const TArray<FWorldRegion>& GetRegions() const { return Regions; }

    UFUNCTION(BlueprintCallable, Category = "World Generator")
    float GetRegionSize() const { return RegionSize; }

    UFUNCTION(BlueprintCallable, Category = "World Generator")
    int32 GetRegionIndexFromLocation(FVector Location) const;

    UFUNCTION(BlueprintCallable, Category = "World Generator")
    TArray<int32> GetNeighborRegionIndices(int32 CenterIdx, int32 Radius = 1) const;

    UFUNCTION(BlueprintCallable, Category = "World Generator")
    bool IsCityRegion(int32 RegionIdx) const;

    // Сид для детерминированной генерации
    UPROPERTY(Config)
    int32 WorldSeed = 123456;

    // Реки
    UPROPERTY()
    TArray<TArray<FVector>> Rivers;

    // Дороги (можно будет использовать для караванов)
    UPROPERTY()
    TArray<TArray<FVector>> Roads;

protected:
    UPROPERTY()
    TArray<FWorldRegion> Regions;

    UPROPERTY()
    TMap<FIntPoint, int32> RegionIndexMap;

    float RegionSize = 100000.f; // 1 км в см
    int32 GridSizeX = 20;
    int32 GridSizeY = 20;
    float NoiseScale = 0.003f;

    FRandomStream RandomStream;

    void GenerateTerrain();
    void GenerateBiomes();
    void GenerateCities();
    void GenerateRivers();
    void GenerateRoads();
    void GeneratePOI();
    void GenerateDungeons();
    void SpawnActors();
};
