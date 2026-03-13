#pragma once

#include "CoreMinimal.h"
#include "ElementAffinityTypes.h"
#include "CrystalTypes.generated.h"

UENUM(BlueprintType)
enum class ECrystalGrade : uint8
{
    Common,
    Pure,
    Divine
};

USTRUCT(BlueprintType)
struct FElementCrystal
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EElementType Element = EElementType::Fire;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ECrystalGrade Grade = ECrystalGrade::Common;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float EffectMultiplier = 1.f;
};
