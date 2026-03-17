#include "VillageSpawner.h"

TArray<FVillageSpawnData> UVillageSpawner::GenerateVillages(UWorldGeneratorSubsystem* WorldGen)
{
    TArray<FVillageSpawnData> Result;
    // 5 стихийных деревень по лору (фиксированные)
    TArray<FString> VillageNames = { TEXT("Fire Village"), TEXT("Water Village"), TEXT("Earth Village"), TEXT("Wind Village"), TEXT("Metal Village") };
    TArray<FVector> VillageLocs = {
        FVector(420000, 900000, 0),
        FVector(600000, 400000, 0),
        FVector(350000, 1550000, 0),
        FVector(1500000, 800000, 0),
        FVector(1100000, 1500000, 0)
    };
    for (int32 i = 0; i < VillageNames.Num(); ++i)
    {
        FVillageSpawnData Village;
        Village.VillageName = VillageNames[i];
        Village.Location = VillageLocs[i];
        Village.Population = FMath::RandRange(50, 200);
        Result.Add(Village);
    }
    return Result;
}
