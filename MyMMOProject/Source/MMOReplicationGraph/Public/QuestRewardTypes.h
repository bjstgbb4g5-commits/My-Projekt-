#pragma once

#include "CoreMinimal.h"
#include "QuestRewardTypes.generated.h"

USTRUCT(BlueprintType)
struct FQuestReward
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Experience = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Gold = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<int32, int32> ItemRewards; // ItemID -> Quantity

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<int32, int32> Reputation; // FactionID -> value
};
