#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "JusticeTypes.generated.h"

USTRUCT(BlueprintType)
struct FJusticeData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 StrikeCount = 0;

    UPROPERTY()
    FDateTime LawMarkExpiration;

    UPROPERTY()
    FDateTime PrisonReleaseTime;

    UPROPERTY()
    FDateTime ExecutionDebuffEnd;
};
