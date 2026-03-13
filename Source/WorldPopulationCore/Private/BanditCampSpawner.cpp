#include "BanditCampSpawner.h"
#include "WorldGeneratorCore/Public/WorldGeneratorTypes.h"

TArray<FBanditCampData> UBanditCampSpawner::GenerateBanditCamps(UWorldGeneratorSubsystem* WorldGen)
{
    TArray<FBanditCampData> Result;
    if (!WorldGen) return Result;

    const auto& Regions = WorldGen->GetRegions();
    for (int32 i = 0; i < Regions.Num(); ++i)
    {
        const FWorldRegion& Region = Regions[i];
        if (Region.HasCity || Region.HasVillage || Region.HasDungeon) continue;
        if (Region.Biome == EBiomeType::Forest || Region.Biome == EBiomeType::Mountains)
        {
            if (FMath::RandRange(0, 100) < 30) // 30% chance
            {
                FBanditCampData Camp;
                Camp.CampID = Result.Num() + 1;
                Camp.Location = FVector(Region.RegionX * WorldGen->GetRegionSize(), Region.RegionY * WorldGen->GetRegionSize(), 0);
                Camp.EnemyCount = FMath::RandRange(5, 15);
                Camp.bCampCleared = false;
                Result.Add(Camp);
            }
        }
    }
    return Result;
}
