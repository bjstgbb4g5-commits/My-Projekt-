
#pragma once

#include "CoreMinimal.h"
#include "CrowdTypes.generated.h"

UENUM(BlueprintType)
enum class ECrowdAgentState : uint8
{
    Idle,
    Walking,
    Avoiding,
    Waiting,
    Talking
};

USTRUCT(BlueprintType)
struct FCrowdAgentData
{
    GENERATED_BODY()

    UPROPERTY()
    AActor* Owner = nullptr;

    UPROPERTY()
    FVector DesiredVelocity = FVector::ZeroVector;

    UPROPERTY()
    FVector CurrentVelocity = FVector::ZeroVector;

    UPROPERTY()
    float Radius = 50.0f;

    UPROPERTY()
    float MaxSpeed = 600.0f;

    UPROPERTY()
    ECrowdAgentState State = ECrowdAgentState::Idle;
};
