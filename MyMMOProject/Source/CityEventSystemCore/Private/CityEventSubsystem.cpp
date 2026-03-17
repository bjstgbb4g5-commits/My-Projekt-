#include "CityEventSubsystem.h"
#include "Engine/World.h"

void UCityEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    Manager = NewObject<UCityEventManager>(this);
    Manager->Initialize(GetWorld());
}

void UCityEventSubsystem::Deinitialize()
{
    if (Manager)
    {
        Manager->Shutdown();
        Manager = nullptr;
    }

    Super::Deinitialize();
}

void UCityEventSubsystem::Tick(float DeltaTime)
{
    if (!IsTickable() || !Manager) return;

    CurrentTime += DeltaTime;
    Manager->Update(DeltaTime, CurrentTime);
}

bool UCityEventSubsystem::IsTickable() const
{
    return GetWorld() && !GetWorld()->bIsTearingDown;
}

TStatId UCityEventSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UCityEventSubsystem, STATGROUP_CityEvent);
}
