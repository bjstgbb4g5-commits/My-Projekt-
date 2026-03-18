
#include "UtilityAISubsystem.h"
#include "Engine/World.h"

void UUtilityAISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    Manager = NewObject<UUtilityAIManager>(this);
    Manager->Initialize(GetWorld());
}

void UUtilityAISubsystem::Deinitialize()
{
    if (Manager)
    {
        Manager->Shutdown();
        Manager = nullptr;
    }
    Super::Deinitialize();
}

void UUtilityAISubsystem::Tick(float DeltaTime)
{
    if (!IsTickable() || !Manager) return;

    CurrentTime += DeltaTime;
    UpdateTimer += DeltaTime;

    if (UpdateTimer >= UPDATE_INTERVAL)
    {
        UpdateTimer = 0.0f;
        Manager->UpdateAllAI();
    }
}

bool UUtilityAISubsystem::IsTickable() const
{
    return GetWorld() && !GetWorld()->bIsTearingDown;
}

TStatId UUtilityAISubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UUtilityAISubsystem, STATGROUP_UtilityAI);
}
