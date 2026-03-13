#pragma once

#include "CoreMinimal.h"
#include "EcosystemTypes.generated.h"

UENUM(BlueprintType)
enum class EEcosystemFaction : uint8
{
    Wildlife,
    Bandits,
    Cultists,
    Monsters
};

UENUM(BlueprintType)
enum class EEcosystemBehavior : uint8
{
    Passive,
    Territorial,
    Predator,
    Patrol,
    Ritual
};

USTRUCT(BlueprintType)
struct FEcosystemZone
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ZoneID = -1;

    UPROPERTY()
    FVector Center;

    UPROPERTY()
    float Radius;

    UPROPERTY()
    EEcosystemFaction OwnerFaction;

    UPROPERTY()
    int32 Population;
};
