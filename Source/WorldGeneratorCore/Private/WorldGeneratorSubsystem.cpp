#include "WorldGeneratorSubsystem.h"
#include "Engine/World.h"
#include "Landscape.h"
#include "LandscapeStreamingProxy.h"
#include "Kismet/GameplayStatics.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"
#include "Math/UnrealMathUtility.h"
#include "Algo/Reverse.h"

UWorldGeneratorSubsystem::UWorldGeneratorSubsystem()
{
}

void UWorldGeneratorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    RandomStream.Initialize(WorldSeed);
}

void UWorldGeneratorSubsystem::Deinitialize()
{
    Regions.Empty();
    RegionIndexMap.Empty();
    Super::Deinitialize();
}

void UWorldGeneratorSubsystem::GenerateWorld()
{
    if (!GetWorld()->IsServer()) return;

    GenerateTerrain();
    GenerateBiomes();
    GenerateCities();
    GenerateRivers();
    GenerateRoads();
    GenerateDungeons();
    GeneratePOI();
    SpawnActors();

    // Построение карты индексов
    for (int32 i = 0; i < Regions.Num(); ++i)
    {
        RegionIndexMap.Add(FIntPoint(Regions[i].RegionX, Regions[i].RegionY), i);
    }
}

void UWorldGeneratorSubsystem::GenerateTerrain()
{
    // Заглушка: создание регионов на основе шума Перлина
    Regions.Empty();
    for (int32 X = 0; X < GridSizeX; ++X)
    {
        for (int32 Y = 0; Y < GridSizeY; ++Y)
        {
            float NoiseX = X * NoiseScale;
            float NoiseY = Y * NoiseScale;
            float Height = FMath::PerlinNoise2D(FVector2D(NoiseX, NoiseY));
            Height = FMath::GetMappedRangeValueClamped(FVector2D(-1,1), FVector2D(0,1), Height);

            FWorldRegion Region;
            Region.RegionX = X;
            Region.RegionY = Y;
            Region.AverageHeight = Height;
            Regions.Add(Region);
        }
    }
}

void UWorldGeneratorSubsystem::GenerateBiomes()
{
    for (FWorldRegion& Region : Regions)
    {
        // Простая логика: высота определяет биом
        if (Region.AverageHeight > 0.8f)
            Region.Biome = EBiomeType::Mountains;
        else if (Region.AverageHeight > 0.6f)
            Region.Biome = EBiomeType::Forest;
        else if (Region.AverageHeight > 0.4f)
            Region.Biome = EBiomeType::Plains;
        else
            Region.Biome = EBiomeType::Ruins; // низкие области – руины
    }
}

void UWorldGeneratorSubsystem::GenerateCities()
{
    // Фиксированные города (в соответствии с лором)
    TArray<FCitySpawnData> FixedCities;
    FixedCities.Add({ "Ironhold", FVector(350000, 1550000, 0) });
    FixedCities.Add({ "Forgeburg", FVector(420000, 900000, 0) });
    FixedCities.Add({ "Greenharbor", FVector(600000, 400000, 0) });
    FixedCities.Add({ "Arcanum", FVector(1500000, 800000, 0) });
    FixedCities.Add({ "NewAurelion", FVector(1000000, 1000000, 0) });
    FixedCities.Add({ "Tenebris", FVector(1100000, 1500000, -20000) });

    for (const FCitySpawnData& City : FixedCities)
    {
        int32 RegionX = FMath::FloorToInt(City.Location.X / RegionSize);
        int32 RegionY = FMath::FloorToInt(City.Location.Y / RegionSize);
        for (FWorldRegion& Region : Regions)
        {
            if (Region.RegionX == RegionX && Region.RegionY == RegionY)
            {
                Region.HasCity = true;
                break;
            }
        }
    }
}

void UWorldGeneratorSubsystem::GenerateRivers()
{
    // Простейший алгоритм: от горных регионов вниз по течению
    TArray<int32> MountainIndices;
    for (int32 i = 0; i < Regions.Num(); ++i)
    {
        if (Regions[i].Biome == EBiomeType::Mountains)
            MountainIndices.Add(i);
    }

    for (int32 StartIdx : MountainIndices)
    {
        TArray<FVector> RiverPoints;
        int32 CurrentIdx = StartIdx;
        int32 Steps = 0;
        const int32 MaxSteps = 30;
        while (Steps < MaxSteps)
        {
            FWorldRegion& Region = Regions[CurrentIdx];
            RiverPoints.Add(FVector(Region.RegionX * RegionSize, Region.RegionY * RegionSize, Region.AverageHeight * 5000));

            // Поиск соседа с наименьшей высотой
            int32 NextIdx = -1;
            float MinHeight = Region.AverageHeight;
            TArray<int32> Neighbors = GetNeighborRegionIndices(CurrentIdx, 1);
            for (int32 N : Neighbors)
            {
                if (Regions[N].AverageHeight < MinHeight)
                {
                    MinHeight = Regions[N].AverageHeight;
                    NextIdx = N;
                }
            }
            if (NextIdx == -1 || NextIdx == CurrentIdx) break;
            CurrentIdx = NextIdx;
            Steps++;
        }
        if (RiverPoints.Num() > 2)
            Rivers.Add(RiverPoints);
    }
}

void UWorldGeneratorSubsystem::GenerateRoads()
{
    // Соединяем города дорогами (простая линия)
    TArray<FVector> CityLocations;
    for (const FWorldRegion& Region : Regions)
    {
        if (Region.HasCity)
        {
            CityLocations.Add(FVector(Region.RegionX * RegionSize, Region.RegionY * RegionSize, 0));
        }
    }

    for (int32 i = 0; i < CityLocations.Num(); ++i)
    {
        for (int32 j = i+1; j < CityLocations.Num(); ++j)
        {
            TArray<FVector> Road;
            Road.Add(CityLocations[i]);
            Road.Add(CityLocations[j]);
            Roads.Add(Road);
        }
    }
}

void UWorldGeneratorSubsystem::GenerateDungeons()
{
    // В каждом регионе с шансом 10% появляется данж
    for (FWorldRegion& Region : Regions)
    {
        if (!Region.HasCity && RandomStream.FRand() < 0.1f)
        {
            Region.HasDungeon = true;
        }
    }
}

void UWorldGeneratorSubsystem::GeneratePOI()
{
    // В каждом регионе добавляем несколько POI (заглушка)
    for (const FWorldRegion& Region : Regions)
    {
        // В реальном коде здесь бы создавались POI
    }
}

void UWorldGeneratorSubsystem::SpawnActors()
{
    // Здесь должен быть спавн акторов на основе сгенерированных данных
}

const FWorldRegion* UWorldGeneratorSubsystem::GetRegionAtLocation(FVector Location) const
{
    int32 X = FMath::FloorToInt(Location.X / RegionSize);
    int32 Y = FMath::FloorToInt(Location.Y / RegionSize);
    if (const int32* Index = RegionIndexMap.Find(FIntPoint(X, Y)))
    {
        return &Regions[*Index];
    }
    return nullptr;
}

int32 UWorldGeneratorSubsystem::GetRegionIndexFromLocation(FVector Location) const
{
    int32 X = FMath::FloorToInt(Location.X / RegionSize);
    int32 Y = FMath::FloorToInt(Location.Y / RegionSize);
    const int32* Index = RegionIndexMap.Find(FIntPoint(X, Y));
    return Index ? *Index : -1;
}

TArray<int32> UWorldGeneratorSubsystem::GetNeighborRegionIndices(int32 CenterIdx, int32 Radius) const
{
    TArray<int32> Result;
    if (!Regions.IsValidIndex(CenterIdx)) return Result;

    int32 CenterX = Regions[CenterIdx].RegionX;
    int32 CenterY = Regions[CenterIdx].RegionY;

    for (int32 dx = -Radius; dx <= Radius; ++dx)
    {
        for (int32 dy = -Radius; dy <= Radius; ++dy)
        {
            if (dx == 0 && dy == 0) continue;
            int32 X = CenterX + dx;
            int32 Y = CenterY + dy;
            if (X >= 0 && X < GridSizeX && Y >= 0 && Y < GridSizeY)
            {
                int32 Idx = X * GridSizeY + Y; // порядок обхода: сначала Y меняется быстрее
                if (Regions.IsValidIndex(Idx))
                    Result.Add(Idx);
            }
        }
    }
    return Result;
}

bool UWorldGeneratorSubsystem::IsCityRegion(int32 RegionIdx) const
{
    return Regions.IsValidIndex(RegionIdx) && Regions[RegionIdx].HasCity;
}
