#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PopulationTypes.h"
#include "WorldGeneratorCore/Public/WorldGeneratorSubsystem.h"
#include "WorldBossSpawner.generated.h"

UCLASS()
class WORLDPOPULATIONCORE_API UWorldBossSpawner : public UObject
{
    GENERATED_BODY()

public:
    TArray<FWorldBossSpawnData> GenerateWorldBosses(UWorldGeneratorSubsystem* WorldGen);
};
