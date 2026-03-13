#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "ChronicleTypes.generated.h"

UENUM(BlueprintType)
enum class EChronicleID : uint8
{
    Chronicle1,
    Chronicle2,
    Chronicle3,
    Chronicle4,
    Chronicle5,
    Chronicle6
};

UENUM(BlueprintType)
enum class ESeasonState : uint8
{
    Preparing,
    Active,
    Finished
};

UENUM(BlueprintType)
enum class ESeasonType : uint8
{
    ApostleWar,
    ApostlePlague,
    ApostleVoid,
    ApostleStorm,
    ApostleFlame,
    NoxSeason
};

USTRUCT(BlueprintType)
struct FChronicleData
{
    GENERATED_BODY()

    UPROPERTY()
    EChronicleID Chronicle = EChronicleID::Chronicle1;

    UPROPERTY()
    int32 CurrentSeason = 1;

    UPROPERTY()
    bool bNoxDefeated = false;

    UPROPERTY()
    FDateTime StartDate;

    UPROPERTY()
    FDateTime EndDate;
};

USTRUCT(BlueprintType)
struct FSeasonData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 SeasonIndex = 0;

    UPROPERTY()
    ESeasonType SeasonType = ESeasonType::ApostleWar;

    UPROPERTY()
    ESeasonState State = ESeasonState::Preparing;

    UPROPERTY()
    FDateTime StartTime;

    UPROPERTY()
    FDateTime EndTime;
};
