#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LiveEventTypes.h"
#include "LiveEventTemplate.generated.h"

UCLASS(BlueprintType)
class WORLDEVENTCORE_API ULiveEventTemplate : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    FString EventName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    ELiveEventType EventType = ELiveEventType::Hunt;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    float DurationMinutes = 120.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    float CooldownMinutes = 240.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    TArray<ELiveEventZone> AllowedZones;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    int32 MaxSimultaneous = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    int32 RewardGold = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    int32 RewardReputation = 0;
};
