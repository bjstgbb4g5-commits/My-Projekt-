#include "NPCPerceptionManager.h"
#include "NPCPerceptionComponent.h"
#include "Engine/World.h"

void UNPCPerceptionManager::Initialize(UWorld* InWorld)
{
    World = InWorld;
}

void UNPCPerceptionManager::Shutdown()
{
    RegisteredNPCs.Empty();
}

void UNPCPerceptionManager::RegisterNPC(AActor* NPC)
{
    if (NPC)
    {
        RegisteredNPCs.AddUnique(NPC);
    }
}

void UNPCPerceptionManager::UnregisterNPC(AActor* NPC)
{
    RegisteredNPCs.Remove(NPC);
}

void UNPCPerceptionManager::BroadcastEvent(const FNPCPerceptionEvent& Event)
{
    for (AActor* NPC : RegisteredNPCs)
    {
        if (!NPC) continue;

        UNPCPerceptionComponent* Perception = NPC->FindComponentByClass<UNPCPerceptionComponent>();
        if (!Perception) continue;

        float Dist = FVector::Dist(Event.Location, NPC->GetActorLocation());

        bool bInRange = false;

        switch (Event.SenseType)
        {
        case ENPCSenseType::Sight:
            bInRange = (Dist <= Perception->SightRadius);
            break;
        case ENPCSenseType::Hearing:
            bInRange = (Dist <= Perception->HearingRadius);
            break;
        case ENPCSenseType::Event:
            bInRange = (Dist <= Perception->HearingRadius * 1.5f);
            break;
        }

        if (bInRange)
        {
            Perception->RegisterPerceptionEvent(Event);
        }
    }
}
