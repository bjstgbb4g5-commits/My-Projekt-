#include "CityEventComponent.h"
#include "Engine/World.h"

UCityEventComponent::UCityEventComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCityEventComponent::StartEvent(ECityEventType EventType, float InStartTime, float InDuration)
{
    CurrentEventType = EventType;
    EventStartTime = InStartTime;
    EventDuration = InDuration;
}

void UCityEventComponent::StopEvent()
{
    CurrentEventType = ECityEventType::None;
    EventStartTime = 0.0f;
    EventDuration = 0.0f;
}

void UCityEventComponent::JoinEvent(ECityEventType EventType)
{
    if (CurrentEventType == ECityEventType::None)
    {
        CurrentEventType = EventType;
        EventStartTime = GetWorld()->GetTimeSeconds();
    }
}

void UCityEventComponent::LeaveEvent()
{
    CurrentEventType = ECityEventType::None;
}

void UCityEventComponent::UpdateEvent(float CurrentTime)
{
}
