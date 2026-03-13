#pragma once

#include "CoreMinimal.h"
#include "CraftingTypes.generated.h"

UENUM(BlueprintType)
enum class ECraftingStationType : uint8
{
    None,
    Blacksmith,
    Woodwork,
    Alchemy,
    Tailoring,
    ArcaneForge
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    Gray,
    Green,
    Blue,
    Red,
    Purple,
    Orange
};

USTRUCT(BlueprintType)
struct FCraftingIngredient
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ItemID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Amount = 1;
};

USTRUCT(BlueprintType)
struct FCraftingRecipe
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RecipeID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ResultItemID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ResultAmount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ECraftingStationType StationType = ECraftingStationType::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CraftTime = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FCraftingIngredient> Ingredients;
};

USTRUCT()
struct FCraftQueueItem
{
    GENERATED_BODY()

    UPROPERTY()
    int32 QueueID = 0;      // уникальный ID элемента очереди

    UPROPERTY()
    int32 RecipeID = 0;

    UPROPERTY()
    float StartTime = 0.f;

    UPROPERTY()
    float CraftDuration = 0.f;
};
