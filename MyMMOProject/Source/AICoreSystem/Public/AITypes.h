#pragma once

#include "CoreMinimal.h"
#include "AITypes.generated.h"

UENUM(BlueprintType)
enum class EAIArchetype : uint8
{
    Beast,
    Bandit,
    Soldier,
    Mage,
    Cultist,
    Elite,
    Boss
};

UENUM(BlueprintType)
enum class EAISquadRole : uint8
{
    Leader,
    Tank,
    Damage,
    Support
};

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
    Phase1,
    Phase2,
    Phase3,
    Phase4,
    Berserk
};

USTRUCT(BlueprintType)
struct FThreatEntry
{
    GENERATED_BODY()

    UPROPERTY()
    AActor* Target = nullptr;

    UPROPERTY()
    float ThreatValue = 0.f;
};
