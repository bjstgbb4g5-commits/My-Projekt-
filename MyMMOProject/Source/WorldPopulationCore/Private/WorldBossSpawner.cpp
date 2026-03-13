#include "WorldBossSpawner.h"

TArray<FWorldBossSpawnData> UWorldBossSpawner::GenerateWorldBosses(UWorldGeneratorSubsystem* WorldGen)
{
    TArray<FWorldBossSpawnData> Result;
    if (!WorldGen) return Result;

    // Генерируем 20 мировых боссов в случайных регионах (не города)
    TArray<int32> EligibleIndices;
    const auto& Regions = WorldGen->GetRegions();
    for (int32 i = 0; i < Regions.Num(); ++i)
    {
        if (!Regions[i].HasCity && !Regions[i].HasVillage)
            EligibleIndices.Add(i);
    }

    for (int32 i = 0; i < 20 && EligibleIndices.Num() > 0; ++i)
    {
        int32 RandIdx = FMath::RandRange(0, EligibleIndices.Num() - 1);
        int32 RegionIdx = EligibleIndices[RandIdx];
        EligibleIndices.RemoveAt(RandIdx);

        FWorldBossSpawnData Boss;
        Boss.BossID = i + 1;
        Boss.Location = FVector(Regions[RegionIdx].RegionX * WorldGen->GetRegionSize(),
                                 Regions[RegionIdx].RegionY * WorldGen->GetRegionSize(), 0);
        Boss.BossClass = nullptr; // будет загружено позже
        Boss.RespawnTime = 86400.f;
        Result.Add(Boss);
    }
    return Result;
}
