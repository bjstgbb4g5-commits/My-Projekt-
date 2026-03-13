#pragma once

#include "CoreMinimal.h"
#include "ItemStats.generated.h"

USTRUCT(BlueprintType)
struct FItemStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Attack = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Defense = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Health = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Mana = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CritChance = 0.0f;
};
