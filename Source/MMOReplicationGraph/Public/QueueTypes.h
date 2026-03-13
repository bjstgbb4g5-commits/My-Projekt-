#pragma once

#include "CoreMinimal.h"
#include "QueueTypes.generated.h"

UENUM(BlueprintType)
enum class EMatchQueueType : uint8
{
    Arena1v1,
    Arena3v3,
    Battleground,
    Dungeon,
    Raid
};

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
    Tank,
    Healer,
    DPS,
    Any
};
