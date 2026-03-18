#include "NPCPerceptionComponent.h"

UNPCPerceptionComponent::UNPCPerceptionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UNPCPerceptionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UNPCPerceptionComponent::RegisterPerceptionEvent(const FNPCPerceptionEvent& Event)
{
    RecentEvents.Add(Event);

    if (RecentEvents.Num() > MAX_RECENT_EVENTS)
    {
        RecentEvents.RemoveAt(0);
    }
}
