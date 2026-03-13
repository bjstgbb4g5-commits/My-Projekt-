#pragma once

#include "CoreMinimal.h"
#include "GuildWarTypes.generated.h"

UENUM(BlueprintType)
enum class EGuildWarState : uint8
{
    Declared,
    Active,
    Finished,
    Cancelled
};

USTRUCT(BlueprintType)
struct FGuildWar
{
    GENERATED_BODY()

    UPROPERTY()
    int32 WarId = -1;

    UPROPERTY()
    int32 AttackerGuildId = -1;

    UPROPERTY()
    int32 DefenderGuildId = -1;

    UPROPERTY()
    FDateTime StartTime;

    UPROPERTY()
    FDateTime EndTime;

    UPROPERTY()
    int32 WarScoreAttacker = 0;

    UPROPERTY()
    int32 WarScoreDefender = 0;

    UPROPERTY()
    EGuildWarState State = EGuildWarState::Declared;
};
