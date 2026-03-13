#pragma once

#include "CoreMinimal.h"
#include "ProfessionTypes.generated.h"

USTRUCT(BlueprintType)
struct FProfessionData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 ProfessionID = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 Level = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float XP = 0.f;
};
