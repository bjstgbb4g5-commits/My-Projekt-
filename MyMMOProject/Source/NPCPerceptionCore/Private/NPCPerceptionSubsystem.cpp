#include "NPCPerceptionSubsystem.h"
#include "Engine/World.h"

void UNPCPerceptionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    Manager = NewObject<UNPCPerceptionManager>(this);
    Manager->Initialize(GetWorld());
}

void UNPCPerceptionSubsystem::Deinitialize()
{
    if (Manager)
    {
        Manager->Shutdown();
        Manager = nullptr;
    }

    Super::Deinitialize();
}
