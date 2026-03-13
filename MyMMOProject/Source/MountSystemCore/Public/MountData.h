#pragma once

#include "CoreMinimal.h"
#include "MountTypes.h"
#include "MountStats.h"
#include "MountGenes.h"
#include "MountData.generated.h"

USTRUCT(BlueprintType)
struct FMountData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 MountID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMountSpecies Species = EMountSpecies::Horse;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMountType Type = EMountType::Travel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMountRarity Rarity = EMountRarity::Common;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FMountStats Stats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FMountGenes Genes;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float CurrentHealth = 0.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsAlive = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float CurrentSpeed = 600.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float RecentDamageTaken = 0.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bPanicking = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float PanicEndTime = 0.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 Level = 1;
};
