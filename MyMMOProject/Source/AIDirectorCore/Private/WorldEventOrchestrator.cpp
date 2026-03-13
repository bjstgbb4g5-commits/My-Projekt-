#include "WorldEventOrchestrator.h"
#include "AIWorldDirectorSubsystem.h"
#include "LiveEventSystemCore/Public/LiveEventSubsystem.h"

void UWorldEventOrchestrator::Initialize(UAIWorldDirectorSubsystem* InDirector)
{
    Director = InDirector;
    if (Director && Director->GetWorld())
    {
        LiveEvents = Director->GetWorld()->GetSubsystem<ULiveEventSubsystem>();
    }
}

void UWorldEventOrchestrator::ConsiderEvents()
{
    // Заглушка: логика выбора события
}

void UWorldEventOrchestrator::TriggerEliteHunt()
{
    if (LiveEvents)
    {
        // LiveEvents->StartEvent(...);
    }
}
