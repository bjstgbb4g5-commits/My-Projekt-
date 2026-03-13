#pragma once

#include "CoreMinimal.h"
#include "MountStats.generated.h"

USTRUCT(BlueprintType)
struct FMountStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Speed = 600.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Stamina = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHealth = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Armor = 20.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CargoCapacity = 0.f;
};
