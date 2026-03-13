#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CraftingTypes.h"
#include "CraftingSaveGame.generated.h"

UCLASS()
class CRAFTINGSYSTEMCORE_API UCraftingSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TMap<int32, FCraftingRecipe> LearnedRecipes; // если игрок изучает рецепты
};
