#pragma once

#include "CoreMinimal.h"
#include "ResourceTypes.h"
#include "CaravanTypes.generated.h"

UENUM(BlueprintType)
enum class ECaravanState : uint8
{
    Preparing,
    Traveling,
    UnderAttack,
    Destroyed,
    Completed
};

UENUM(BlueprintType)
enum class ECaravanRiskLevel : uint8
{
    Low,
    Medium,
    High,
    Extreme
};

USTRUCT(BlueprintType)
struct FCaravanCargo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EWorldResourceType ResourceType = EWorldResourceType::Wood;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Amount = 0;
};

USTRUCT(BlueprintType)
struct FCaravanData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 CaravanID = -1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName OwnerGuild;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 OriginCity = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 DestinationCity = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FVector CurrentLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Progress = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    ECaravanState State = ECaravanState::Preparing;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FCaravanCargo> Cargo;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 CurrentTerritoryID = -1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    ECaravanRiskLevel RiskLevel = ECaravanRiskLevel::Low;
};
