#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PopulationTypes.h"
#include "WorldGeneratorCore/Public/WorldGeneratorSubsystem.h"
#include "NPCPatrolSpawner.generated.h"

UCLASS()
class WORLDPOPULATIONCORE_API UNPCPatrolSpawner : public UObject
{
    GENERATED_BODY()

public:
    TArray<FNPCPatrolData> GenerateNPCPatrols(UWorldGeneratorSubsystem* WorldGen);
};
