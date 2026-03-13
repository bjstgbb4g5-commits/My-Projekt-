#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "CityPoliticsTypes.generated.h"

USTRUCT(BlueprintType)
struct FCityRelation
{
    GENERATED_BODY()

    UPROPERTY()
    int32 CityA = 0;

    UPROPERTY()
    int32 CityB = 0;

    UPROPERTY()
    float RelationValue = 0.f;

    UPROPERTY()
    bool bWarActive = false;
};
