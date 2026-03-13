#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "AIDirectorTypes.generated.h"

UENUM(BlueprintType)
enum class EEliteModule : uint8
{
    Aggressive,
    Guardian,
    Hunter,
    Assassin,
    Caster,
    Berserker
};

USTRUCT()
struct FServerLearningProfile
{
    GENERATED_BODY()

    UPROPERTY()
    float AvgDPS = 0.f;

    UPROPERTY()
    float AvgTTK = 0.f;

    UPROPERTY()
    float RangedUsage = 0.f;

    UPROPERTY()
    float MeleeUsage = 0.f;

    UPROPERTY()
    float DodgeFrequency = 0.f;

    UPROPERTY()
    float BlockFrequency = 0.f;

    UPROPERTY()
    TMap<FString, int32> ZonePopularity;
};

USTRUCT()
struct FPlayerBehaviorData
{
    GENERATED_BODY()

    UPROPERTY()
    float DPS = 0.f;

    UPROPERTY()
    float TTK = 0.f;

    UPROPERTY()
    float SurvivalTime = 0.f;

    UPROPERTY()
    float DamageTaken = 0.f;

    UPROPERTY()
    int32 AbilityUses = 0;

    UPROPERTY()
    int32 DodgeCount = 0;

    UPROPERTY()
    int32 BlockCount = 0;

    UPROPERTY()
    FDateTime LastUpdate;
};

USTRUCT()
struct FPlayerEfficiency
{
    GENERATED_BODY()

    UPROPERTY()
    float Efficiency = 0.f;

    UPROPERTY()
    FDateTime LastComputed;
};

USTRUCT()
struct FMobAdaptation
{
    GENERATED_BODY()

    UPROPERTY()
    float HealthMultiplier = 1.0f;

    UPROPERTY()
    float DamageMultiplier = 1.0f;

    UPROPERTY()
    float AggressionMultiplier = 1.0f;

    UPROPERTY()
    TArray<EEliteModule> Modules;
};
