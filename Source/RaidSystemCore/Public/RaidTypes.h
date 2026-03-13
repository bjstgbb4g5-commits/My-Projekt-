#pragma once

#include "CoreMinimal.h"
#include "RaidTypes.generated.h"

UENUM(BlueprintType)
enum class ERaidDifficulty : uint8
{
    Normal,
    Heroic,
    Mythic
};

UENUM(BlueprintType)
enum class ERaidState : uint8
{
    Waiting,
    Active,
    BossFight,
    Completed,
    Failed
};
