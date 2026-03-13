#pragma once

#include "CoreMinimal.h"
#include "ElementAffinityTypes.generated.h"

UENUM(BlueprintType)
enum class EElementType : uint8
{
    Fire,
    Water,
    Earth,
    Wind,
    Metal
};

UENUM(BlueprintType)
enum class EAffinityLevel : uint8
{
    Touch = 1,
    Resonance,
    Harmony,
    Avatar
};

USTRUCT(BlueprintType)
struct FElementAffinity
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EElementType Element = EElementType::Fire;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 Level = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float AffinityXP = 0.f;
};
