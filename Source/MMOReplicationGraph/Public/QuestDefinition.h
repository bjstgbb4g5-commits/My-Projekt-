#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestObjectiveTypes.h"
#include "QuestRewardTypes.h"
#include "QuestDefinition.generated.h"

USTRUCT(BlueprintType)
struct FQuestDefinition : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 QuestID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText QuestName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FQuestObjective> Objectives;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FQuestReward Rewards;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RequiredLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RequiredFactionID = -1; // -1 = любая

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RequiredFactionStanding = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RegionID = -1; // где доступен

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ChroniclePhase = 1; // минимальная фаза хроники

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bRepeatable = false;
};
