#include "NPCDensityZoneComponent.h"
#include "NPCDensitySubsystem.h"
#include "Engine/World.h"

UNPCDensityZoneComponent::UNPCDensityZoneComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UNPCDensityZoneComponent::BeginPlay()
{
    Super::BeginPlay();

    if (UNPCDensitySubsystem* Subsystem = GetWorld()->GetSubsystem<UNPCDensitySubsystem>())
    {
        Subsystem->RegisterZone(this);
    }
}

void UNPCDensityZoneComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UNPCDensitySubsystem* Subsystem = GetWorld()->GetSubsystem<UNPCDensitySubsystem>())
    {
        Subsystem->UnregisterZone(this);
    }

    Super::EndPlay(EndPlayReason);
}
