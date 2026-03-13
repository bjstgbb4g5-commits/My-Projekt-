#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TerritoryTypes.h"
#include "TerritorySaveGame.generated.h"

UCLASS()
class WORLDTERRITORYSYSTEMCORE_API UTerritorySaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<FTerritoryData> Territories;
};
