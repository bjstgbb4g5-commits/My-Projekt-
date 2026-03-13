#pragma once

#include "CoreMinimal.h"
#include "DungeonAITypes.generated.h"

UENUM(BlueprintType)
enum class EDungeonMobRole : uint8
{
    Tank,
    Melee,
    Ranged,
    Assassin,
    Caster,
    Support
};

UENUM(BlueprintType)
enum class EDungeonEventType : uint8
{
    None,
    Ambush,
    ElitePatrol,
    TrapRoom,
    HiddenBoss,
    TreasureGoblin
};

USTRUCT()
struct FDungeonGroupProfile
{
    GENERATED_BODY()

    UPROPERTY()
    float GroupDPS = 0.f;

    UPROPERTY()
    float HealRate = 0.f;

    UPROPERTY()
    int32 Deaths = 0;

    UPROPERTY()
    float DodgeFrequency = 0.f;

    UPROPERTY()
    float AverageTTK = 0.f;

    UPROPERTY()
    FDateTime LastUpdate;
};

USTRUCT()
struct FDungeonEncounter
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<int32> MobIDs;

    UPROPERTY()
    int32 EliteCount = 0;

    UPROPERTY()
    EDungeonEventType Event = EDungeonEventType::None;
};

USTRUCT()
struct FDungeonLearningProfile
{
    GENERATED_BODY()

    UPROPERTY()
    float AvgCompletionTime = 0.f;

    UPROPERTY()
    float AvgDeaths = 0.f;

    UPROPERTY()
    float SuccessRate = 1.0f;

    UPROPERTY()
    int32 TotalRuns = 0;

    UPROPERTY()
    FDateTime LastUpdate;
};
