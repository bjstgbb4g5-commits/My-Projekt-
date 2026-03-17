#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PopulationTypes.h"
#include "WorldGeneratorCore/Public/WorldGeneratorSubsystem.h"
#include "BanditCampSpawner.generated.h"

UCLASS()
class WORLDPOPULATIONCORE_API UBanditCampSpawner : public UObject
{
    GENERATED_BODY()

public:
    TArray<FBanditCampData> GenerateBanditCamps(UWorldGeneratorSubsystem* WorldGen);
};
