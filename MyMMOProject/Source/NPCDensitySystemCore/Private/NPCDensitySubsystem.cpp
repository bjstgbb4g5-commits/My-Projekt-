#include "NPCDensitySubsystem.h"
#include "Engine/World.h"

void UNPCDensitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    Manager = NewObject<UNPCDensityManager>(this);
    Manager->Initialize(GetWorld());
}

void UNPCDensitySubsystem::Deinitialize()
{
    if (Manager)
    {
        Manager->Shutdown();
        Manager = nullptr;
    }
    Super::Deinitialize();
}

void UNPCDensitySubsystem::Tick(float DeltaTime)
{
    if (!IsTickable() || !Manager) return;

    CurrentTime += DeltaTime;
    Manager->UpdateZones(DeltaTime, CurrentTime);
}

bool UNPCDensitySubsystem::IsTickable() const
{
    return GetWorld() && !GetWorld()->bIsTearingDown;
}

TStatId UNPCDensitySubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UNPCDensitySubsystem, STATGROUP_NPCDensity);
}

void UNPCDensitySubsystem::RegisterZone(UNPCDensityZoneComponent* Zone)
{
    if (Manager)
    {
        Manager->RegisterZone(Zone);
    }
}

void UNPCDensitySubsystem::UnregisterZone(UNPCDensityZoneComponent* Zone)
{
    if (Manager)
    {
        Manager->UnregisterZone(Zone);
    }
}
