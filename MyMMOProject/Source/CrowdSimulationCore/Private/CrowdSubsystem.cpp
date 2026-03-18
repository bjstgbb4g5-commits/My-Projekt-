
#include "CrowdSubsystem.h"
#include "Engine/World.h"

void UCrowdSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    Manager = NewObject<UCrowdManager>(this);
    Manager->Initialize(GetWorld());
}

void UCrowdSubsystem::Deinitialize()
{
    if (Manager)
    {
        Manager->Shutdown();
        Manager = nullptr;
    }
    Super::Deinitialize();
}

void UCrowdSubsystem::Tick(float DeltaTime)
{
    if (!IsTickable() || !Manager) return;

    CurrentTime += DeltaTime;
    UpdateTimer += DeltaTime;

    if (UpdateTimer >= UPDATE_INTERVAL)
    {
        UpdateTimer = 0.0f;
        Manager->UpdateCrowd(UPDATE_INTERVAL);
    }
}

bool UCrowdSubsystem::IsTickable() const
{
    return GetWorld() && !GetWorld()->bIsTearingDown;
}

TStatId UCrowdSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UCrowdSubsystem, STATGROUP_CrowdSimulation);
}
