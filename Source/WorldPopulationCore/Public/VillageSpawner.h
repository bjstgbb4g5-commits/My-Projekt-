#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PopulationTypes.h"
#include "WorldGeneratorCore/Public/WorldGeneratorSubsystem.h"
#include "VillageSpawner.generated.h"

UCLASS()
class WORLDPOPULATIONCORE_API UVillageSpawner : public UObject
{
    GENERATED_BODY()

public:
    TArray<FVillageSpawnData> GenerateVillages(UWorldGeneratorSubsystem* WorldGen);
};
