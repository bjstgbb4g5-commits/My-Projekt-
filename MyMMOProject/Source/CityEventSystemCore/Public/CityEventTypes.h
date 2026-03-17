#pragma once

#include "CoreMinimal.h"
#include "CityEventTypes.generated.h"

UENUM(BlueprintType)
enum class ECityEventType : uint8
{
    None,
    StreetFight,
    Pickpocket,
    StreetPerformance,
    MerchantDispute,
    GuardPatrol,
    PublicSpeech,
    ThiefChase,
    DrunkBrawl
};

USTRUCT(BlueprintType)
struct FCityEvent
{
    GENERATED_BODY()

    UPROPERTY()
    int32 EventId = -1;

    UPROPERTY()
    ECityEventType EventType = ECityEventType::None;

    UPROPERTY()
    FVector Location = FVector::ZeroVector;

    UPROPERTY()
    float StartTime = 0.0f;

    UPROPERTY()
    float Duration = 30.0f;

    UPROPERTY()
    TArray<AActor*> Participants;

    UPROPERTY()
    bool bActive = true;

    static constexpr float EVENT_RADIUS = 1500.0f;
};
