#pragma once

#include "CoreMinimal.h"
#include "DungeonTypes.generated.h"

UENUM(BlueprintType)
enum class EDungeonDifficulty : uint8
{
    Normal,
    Elite,
    Mythic
};

UENUM(BlueprintType)
enum class EDungeonState : uint8
{
    Waiting,
    Active,
    BossFight,
    Completed,
    Failed
};

UENUM(BlueprintType)
enum class EDungeonRoomType : uint8
{
    Entrance,
    Combat,
    Elite,
    Puzzle,
    Boss
};
