#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ResourceTypes.h"
#include "ResourceSaveGame.generated.h"

UCLASS()
class RESOURCEECONOMYSYSTEMCORE_API UResourceSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<FResourceNode> SavedNodes;

    UPROPERTY()
    TMap<int32, float> SavedAccumulators;

    UPROPERTY()
    int32 SavedNextNodeID = 1;
};
