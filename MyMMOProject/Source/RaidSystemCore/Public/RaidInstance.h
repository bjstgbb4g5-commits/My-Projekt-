#pragma once

#include "CoreMinimal.h"
#include "RaidTypes.h"
#include "RaidInstance.generated.h"

USTRUCT(BlueprintType)
struct FRaidInstance
{
    GENERATED_BODY()

    UPROPERTY()
    int32 RaidID = -1;

    UPROPERTY()
    ERaidDifficulty Difficulty = ERaidDifficulty::Normal;

    UPROPERTY()
    ERaidState State = ERaidState::Waiting;

    UPROPERTY()
    TArray<class APlayerState*> Players;

    UPROPERTY()
    int32 BossIndex = 0;

    UPROPERTY()
    int32 BossCount = 3;
};
