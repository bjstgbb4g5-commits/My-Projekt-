#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RaidDifficulty.generated.h"

USTRUCT(BlueprintType)
struct FRaidDifficultyData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 BossCount = 3;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float HealthMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DamageMultiplier = 1.0f;
};
