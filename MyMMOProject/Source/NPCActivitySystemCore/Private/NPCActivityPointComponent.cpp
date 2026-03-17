#include "NPCActivityPointComponent.h"
#include "NPCActivitySubsystem.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UNPCActivityPointComponent::UNPCActivityPointComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UNPCActivityPointComponent::BeginPlay()
{
    Super::BeginPlay();

    if (UNPCActivitySubsystem* Subsystem = GetWorld()->GetSubsystem<UNPCActivitySubsystem>())
    {
        Subsystem->RegisterActivityPoint(this);
    }
}

void UNPCActivityPointComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UNPCActivitySubsystem* Subsystem = GetWorld()->GetSubsystem<UNPCActivitySubsystem>())
    {
        Subsystem->UnregisterActivityPoint(this);
    }

    Super::EndPlay(EndPlayReason);
}

void UNPCActivityPointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if ENABLE_DRAW_DEBUG
    if (GetWorld() && GetWorld()->IsGameWorld())
    {
        FColor Color = HasFreeSlot(ENPCActivityType::None) ? FColor::Green : FColor::Red;
        DrawDebugSphere(GetWorld(), GetComponentLocation(), InteractionRadius, 12, Color);
    }
#endif
}

bool UNPCActivityPointComponent::HasFreeSlot(ENPCActivityType Activity) const
{
    for (const FActivitySlot& Slot : Slots)
    {
        if (Slot.ActivityType == Activity && !Slot.OccupyingNPC.IsValid())
        {
            return true;
        }
    }
    return false;
}

bool UNPCActivityPointComponent::TryReserveSlot(AActor* NPC, ENPCActivityType Activity)
{
    if (!NPC) return false;

    for (FActivitySlot& Slot : Slots)
    {
        if (Slot.ActivityType == Activity && !Slot.OccupyingNPC.IsValid())
        {
            Slot.OccupyingNPC = NPC;
            return true;
        }
    }
    return false;
}

bool UNPCActivityPointComponent::ReleaseSlot(AActor* NPC)
{
    if (!NPC) return false;

    for (FActivitySlot& Slot : Slots)
    {
        if (Slot.OccupyingNPC.Get() == NPC)
        {
            Slot.OccupyingNPC = nullptr;
            return true;
        }
    }
    return false;
}
