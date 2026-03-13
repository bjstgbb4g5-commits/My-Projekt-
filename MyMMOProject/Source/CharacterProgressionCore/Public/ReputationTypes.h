#pragma once

#include "CoreMinimal.h"
#include "ReputationTypes.generated.h"

USTRUCT(BlueprintType)
struct FReputationData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 FactionID = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 ReputationLevel = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float ReputationXP = 0.f;
};
