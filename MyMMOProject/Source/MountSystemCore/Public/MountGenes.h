#pragma once

#include "CoreMinimal.h"
#include "MountGenes.generated.h"

USTRUCT(BlueprintType)
struct FMountGenes
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpeedGene = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StaminaGene = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StrengthGene = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CargoGene = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TemperGene = 1.f;
};
