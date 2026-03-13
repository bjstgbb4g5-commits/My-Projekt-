#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldGeneratorTypes.h"
#include "RiverGenerator.generated.h"

UCLASS()
class WORLDGENERATORCORE_API URiverGenerator : public UObject
{
    GENERATED_BODY()

public:
    TArray<TArray<FVector>> GenerateRivers(const TArray<FWorldRegion>& Regions, int32 GridSizeX, int32 GridSizeY, float RegionSize, FRandomStream& Random);
};
