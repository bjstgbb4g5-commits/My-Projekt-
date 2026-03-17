#include "NPCActivitySubsystem.h"
#include "Engine/World.h"

void UNPCActivitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    Manager = NewObject<UNPCActivityManager>(this);
    Manager->Initialize(GetWorld());
}

void UNPCActivitySubsystem::Deinitialize()
{
    if (Manager)
    {
        Manager->Shutdown();
        Manager = nullptr;
    }
    Super::Deinitialize();
}

void UNPCActivitySubsystem::Tick(float DeltaTime)
{
    if (!IsTickable() || !Manager) return;

    CurrentTime += DeltaTime;
    Manager->Update(DeltaTime);
}

bool UNPCActivitySubsystem::IsTickable() const
{
    return GetWorld() && !GetWorld()->bIsTearingDown;
}

TStatId UNPCActivitySubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UNPCActivitySubsystem, STATGROUP_NPCActivity);
}

void UNPCActivitySubsystem::RegisterActivityPoint(UNPCActivityPointComponent* Point)
{
    if (Manager)
    {
        Manager->RegisterActivityPoint(Point);
    }
}

void UNPCActivitySubsystem::UnregisterActivityPoint(UNPCActivityPointComponent* Point)
{
    if (Manager)
    {
        Manager->UnregisterActivityPoint(Point);
    }
}
