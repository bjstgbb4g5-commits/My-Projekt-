#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LiveEventTypes.h"
#include "LiveEventWorldState.generated.h"

UCLASS()
class LIVEEVENTSYSTEMCORE_API ALiveEventWorldState : public AActor
{
    GENERATED_BODY()

public:
    ALiveEventWorldState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(ReplicatedUsing = OnRep_ActiveEvents)
    TMap<int32, FLiveEventInstance> ActiveEvents;

    UFUNCTION()
    void OnRep_ActiveEvents();
};
