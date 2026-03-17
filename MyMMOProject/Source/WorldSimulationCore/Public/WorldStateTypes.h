#pragma once

#include "CoreMinimal.h"
#include "WorldStateTypes.generated.h"

USTRUCT(BlueprintType)
struct FWorldState
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Population = 0;

    UPROPERTY()
    float EconomyHealth = 1.0f;

    UPROPERTY()
    float CrimeLevel = 0.0f;

    UPROPERTY()
    float PoliticalTension = 0.0f;

    UPROPERTY()
    float EconomicPressure = 0.0f;

    UPROPERTY()
    float UnrestLevel = 0.0f;
};
