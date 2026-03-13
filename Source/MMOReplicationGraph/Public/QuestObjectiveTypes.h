#pragma once

#include "CoreMinimal.h"
#include "QuestTypes.h"
#include "QuestObjectiveTypes.generated.h"

USTRUCT(BlueprintType)
struct FQuestObjectiveDef
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EQuestObjectiveType ObjectiveType = EQuestObjectiveType::KillTarget;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 TargetID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RequiredCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RegionID = -1;
};
