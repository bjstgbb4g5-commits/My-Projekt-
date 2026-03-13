#pragma once

#include "CoreMinimal.h"
#include "QuestTypes.generated.h"

UENUM(BlueprintType)
enum class EQuestState : uint8
{
    Inactive,
    Available,
    Active,
    Completed,
    Failed
};

UENUM(BlueprintType)
enum class EQuestObjectiveType : uint8
{
    KillTarget,
    CollectItem,
    VisitLocation,
    InteractNPC,
    CraftItem,
    EscortNPC,
    WorldEventParticipation
};
