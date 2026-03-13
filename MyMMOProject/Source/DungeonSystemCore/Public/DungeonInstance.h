#pragma once

#include "CoreMinimal.h"
#include "DungeonTypes.h"
#include "DungeonInstance.generated.h"

USTRUCT(BlueprintType)
struct FDungeonInstance
{
    GENERATED_BODY()

    UPROPERTY()
    int32 InstanceID = -1;

    UPROPERTY()
    EDungeonDifficulty Difficulty = EDungeonDifficulty::Normal;

    UPROPERTY()
    EDungeonState State = EDungeonState::Waiting;

    UPROPERTY()
    TArray<class APlayerState*> Players;

    UPROPERTY()
    int32 CurrentRoom = 0;

    UPROPERTY()
    bool bBossAlive = true;
};
