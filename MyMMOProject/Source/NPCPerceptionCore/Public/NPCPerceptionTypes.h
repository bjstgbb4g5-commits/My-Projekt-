#pragma once

#include "CoreMinimal.h"
#include "NPCPerceptionTypes.generated.h"

UENUM(BlueprintType)
enum class ENPCSenseType : uint8
{
    Sight,
    Hearing,
    Event
};

USTRUCT(BlueprintType)
struct FNPCPerceptionEvent
{
    GENERATED_BODY()

    UPROPERTY()
    ENPCSenseType SenseType = ENPCSenseType::Sight;

    UPROPERTY()
    FVector Location = FVector::ZeroVector;

    UPROPERTY()
    AActor* SourceActor = nullptr;

    UPROPERTY()
    float TimeStamp = 0.0f;
};
