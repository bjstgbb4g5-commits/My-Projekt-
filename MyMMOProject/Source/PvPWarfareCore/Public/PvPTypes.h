#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "PvPTypes.generated.h"

UENUM(BlueprintType)
enum class EPvPBattleType : uint8
{
    Caravan,
    Outpost,
    Fortress,
    CitySiege
};

UENUM(BlueprintType)
enum class EPvPBattleState : uint8
{
    Waiting,
    Preparing,
    Active,
    Finished
};

UENUM(BlueprintType)
enum class EPvPBattleResult : uint8
{
    AttackersWin,
    DefendersWin,
    Timeout
};

UENUM(BlueprintType)
enum class ESiegeMachineType : uint8
{
    BatteringRam,
    Catapult,
    Ballista,
    ArcaneCannon
};

USTRUCT(BlueprintType)
struct FPvPBattle
{
    GENERATED_BODY()

    UPROPERTY()
    int32 BattleID = -1;

    UPROPERTY()
    EPvPBattleType BattleType = EPvPBattleType::Caravan;

    UPROPERTY()
    EPvPBattleState State = EPvPBattleState::Waiting;

    UPROPERTY()
    TArray<class APlayerState*> Attackers;

    UPROPERTY()
    TArray<class APlayerState*> Defenders;

    UPROPERTY()
    int32 MaxAttackers = 0;

    UPROPERTY()
    int32 MaxDefenders = 0;

    UPROPERTY()
    float BattleDuration = 0.f; // seconds

    UPROPERTY()
    float TimeRemaining = 0.f;

    UPROPERTY()
    FDateTime StartTime;
};

USTRUCT(BlueprintType)
struct FSiegeMachineData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESiegeMachineType Type = ESieMachineType::BatteringRam;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHealth = 1000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReloadTime = 5.f;
};

USTRUCT(BlueprintType)
struct FSiegeLimitConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxRams = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxCatapults = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxBallistas = 6;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxArcaneCannons = 2;
};

USTRUCT()
struct FPvPBattleLog
{
    GENERATED_BODY()

    UPROPERTY()
    int32 BattleID = 0;

    UPROPERTY()
    EPvPBattleType Type;

    UPROPERTY()
    int32 AttackersCount = 0;

    UPROPERTY()
    int32 DefendersCount = 0;

    UPROPERTY()
    float Duration = 0.f;

    UPROPERTY()
    EPvPBattleResult Result;
};
