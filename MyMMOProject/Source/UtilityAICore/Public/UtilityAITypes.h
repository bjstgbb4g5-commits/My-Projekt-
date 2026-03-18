
#pragma once

#include "CoreMinimal.h"
#include "UtilityAITypes.generated.h"

UENUM(BlueprintType)
enum class ENPCUtilityActionType : uint8
{
    Idle,
    Work,
    Eat,
    Sleep,
    Socialize,
    InvestigateEvent,
    Flee,
    TalkToPlayer
};

USTRUCT(BlueprintType)
struct FNPCUtilityActionScore
{
    GENERATED_BODY()

    UPROPERTY()
    ENPCUtilityActionType ActionType = ENPCUtilityActionType::Idle;

    UPROPERTY()
    float Score = 0.0f;
};
