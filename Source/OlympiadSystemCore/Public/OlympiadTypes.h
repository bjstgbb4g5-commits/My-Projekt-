#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "OlympiadTypes.generated.h"

UENUM(BlueprintType)
enum class EOlympiadState : uint8
{
    Closed,
    Registration,
    Active,
    Finished
};

UENUM(BlueprintType)
enum class EOlympiadMatchState : uint8
{
    Waiting,
    Fighting,
    Completed
};

USTRUCT(BlueprintType)
struct FOlympiadPlayerData
{
    GENERATED_BODY()

    UPROPERTY()
    FUniqueNetIdRepl PlayerID;

    UPROPERTY()
    int32 Rating = 1000;

    UPROPERTY()
    int32 Wins = 0;

    UPROPERTY()
    int32 Losses = 0;

    UPROPERTY()
    int32 WeeklyPoints = 0;
};

USTRUCT()
struct FOlympiadMatch
{
    GENERATED_BODY()

    UPROPERTY()
    int32 MatchID = -1;

    UPROPERTY()
    FUniqueNetIdRepl PlayerA;

    UPROPERTY()
    FUniqueNetIdRepl PlayerB;

    UPROPERTY()
    EOlympiadMatchState State = EOlympiadMatchState::Waiting;

    UPROPERTY()
    FDateTime StartTime;
};
