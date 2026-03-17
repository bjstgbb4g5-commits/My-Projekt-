#include "NPCLifeSubsystem.h"
#include "WorldSimulationCore/Public/WorldSimulationSubsystem.h"
#include "Engine/World.h"

void UNPCLifeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    WorldSim = GetWorld()->GetSubsystem<UWorldSimulationSubsystem>();
    if (!WorldSim)
    {
        UE_LOG(LogTemp, Error, TEXT("NPCLifeSubsystem requires WorldSimulationSubsystem!"));
    }

    Manager = NewObject<UNPCLifeManager>(this);
    Manager->Initialize();
}

void UNPCLifeSubsystem::Deinitialize()
{
    if (Manager)
    {
        Manager->Shutdown();
        Manager = nullptr;
    }
    Super::Deinitialize();
}

void UNPCLifeSubsystem::Tick(float DeltaTime)
{
    if (!IsTickable()) return;

    CurrentHour += DeltaTime * HOURS_PER_REAL_SECOND;
    if (CurrentHour >= 24.0f)
        CurrentHour -= 24.0f;

    if (Manager)
    {
        Manager->UpdateSimulation(DeltaTime, CurrentHour);
    }
}

bool UNPCLifeSubsystem::IsTickable() const
{
    return GetWorld() && !GetWorld()->bIsTearingDown;
}

TStatId UNPCLifeSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UNPCLifeSubsystem, STATGROUP_Tickables);
}

void UNPCLifeSubsystem::RegisterNPC(AActor* NPC, const FNPCLifeData& Data)
{
    if (Manager)
    {
        Manager->RegisterNPC(NPC, Data);
    }
}

void UNPCLifeSubsystem::UnregisterNPC(AActor* NPC)
{
    if (Manager)
    {
        Manager->UnregisterNPC(NPC);
    }
}
