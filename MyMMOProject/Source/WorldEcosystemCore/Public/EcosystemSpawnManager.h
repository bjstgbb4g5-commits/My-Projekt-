#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EcosystemTypes.h"
#include "EcosystemSpawnManager.generated.h"

UCLASS()
class WORLDECOSYSTEMCORE_API UEcosystemSpawnManager : public UObject
{
    GENERATED_BODY()

public:
    void SpawnCreatures(FEcosystemZone& Zone);
    void DespawnFarCreatures();
};
