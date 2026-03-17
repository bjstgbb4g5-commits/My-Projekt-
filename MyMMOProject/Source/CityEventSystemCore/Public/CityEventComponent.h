#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CityEventTypes.h"
#include "CityEventComponent.generated.h"

UCLASS(ClassGroup=(NPC), meta=(BlueprintSpawnableComponent))
class CITYEVENTSYSTEMCORE_API UCityEventComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCityEventComponent();

    void StartEvent(ECityEventType EventType, float InStartTime, float InDuration);
    void StopEvent();
    void JoinEvent(ECityEventType EventType);
    void LeaveEvent();

    bool IsInEvent() const { return CurrentEventType != ECityEventType::None; }
    ECityEventType GetCurrentEventType() const { return CurrentEventType; }

    void UpdateEvent(float CurrentTime);

private:
    ECityEventType CurrentEventType = ECityEventType::None;
    float EventStartTime = 0.0f;
    float EventDuration = 0.0f;
};
