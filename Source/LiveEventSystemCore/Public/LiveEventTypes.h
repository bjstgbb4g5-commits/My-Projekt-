#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "LiveEventTypes.generated.h"

UENUM(BlueprintType)
enum class ELiveEventType : uint8
{
    Invasion,
    Hunt,
    Dungeon,
    Crisis,
    Economy,
    PvP,
    Festival,
    World,
    Rare
};

UENUM(BlueprintType)
enum class ELiveEventState : uint8
{
    Pending,
    Active,
    Completed,
    Failed
};

UENUM(BlueprintType)
enum class ELiveEventZone : uint8
{
    Green,
    Blue,
    Red,
    Black
};

USTRUCT(BlueprintType)
struct FLiveEventInstance
{
    GENERATED_BODY()

    UPROPERTY()
    int32 EventID = -1;

    UPROPERTY()
    FString TemplateName;

    UPROPERTY()
    FVector Location;

    UPROPERTY()
    ELiveEventState State = ELiveEventState::Pending;

    UPROPERTY()
    FDateTime StartTime;

    UPROPERTY()
    FDateTime EndTime;
};
