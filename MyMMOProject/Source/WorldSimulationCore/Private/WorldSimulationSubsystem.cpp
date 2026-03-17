#include "WorldSimulationSubsystem.h"
#include "WorldEconomySubsystem.h"
#include "WorldEventSubsystem.h"
#include "WorldPopulationSubsystem.h"
#include "AIWorldDirectorSubsystem.h"

void UWorldSimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UWorld* World = GetWorld();
    check(World);

    EconomySubsystem = World->GetSubsystem<UWorldEconomySubsystem>();
    EventSubsystem = World->GetSubsystem<UWorldEventSubsystem>();
    PopulationSubsystem = World->GetSubsystem<UWorldPopulationSubsystem>();
    AIDirectorSubsystem = World->GetSubsystem<UAIWorldDirectorSubsystem>();

    bSimulationRunning = false;
}

void UWorldSimulationSubsystem::Deinitialize()
{
    EconomySubsystem = nullptr;
    EventSubsystem = nullptr;
    PopulationSubsystem = nullptr;
    AIDirectorSubsystem = nullptr;

    Super::Deinitialize();
}

void UWorldSimulationSubsystem::Tick(float DeltaTime)
{
    if (!IsTickable())
        return;

    TickManager.Tick(DeltaTime);

    const int32 MAX_STEPS_PER_TICK = 4;
    int32 StepsThisTick = 0;

    while (TickManager.ShouldRunStep() && StepsThisTick < MAX_STEPS_PER_TICK)
    {
        RunSimulationStep();
        StepsThisTick++;
    }

    if (StepsThisTick == MAX_STEPS_PER_TICK)
    {
        TickManager.ResetAccumulator();
    }
}

bool UWorldSimulationSubsystem::IsTickable() const
{
    return bSimulationRunning
        && GetWorld()
        && !GetWorld()->bIsTearingDown;
}

TStatId UWorldSimulationSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UWorldSimulationSubsystem, STATGROUP_WorldSimulation);
}

void UWorldSimulationSubsystem::StartSimulation()
{
    bSimulationRunning = true;
}

void UWorldSimulationSubsystem::PauseSimulation()
{
    bSimulationRunning = false;
}

void UWorldSimulationSubsystem::RunSimulationStep()
{
    const float StepDelta = FWorldTickManager::FIXED_STEP;

    if (EconomySubsystem)
        UpdateEconomy(StepDelta);
    if (PopulationSubsystem)
        UpdatePopulation(StepDelta);
    if (EventSubsystem)
        UpdateEvents(StepDelta);
    if (AIDirectorSubsystem)
        UpdateAIDirector(StepDelta);
}

void UWorldSimulationSubsystem::UpdateEconomy(float DeltaTime)
{
    // EconomySubsystem->UpdateEconomy(DeltaTime, WorldState);
}

void UWorldSimulationSubsystem::UpdatePopulation(float DeltaTime)
{
    // PopulationSubsystem->UpdatePopulation(DeltaTime, WorldState);
}

void UWorldSimulationSubsystem::UpdateEvents(float DeltaTime)
{
    // EventSubsystem->UpdateEvents(DeltaTime, WorldState);
}

void UWorldSimulationSubsystem::UpdateAIDirector(float DeltaTime)
{
    // AIDirectorSubsystem->UpdateDirector(DeltaTime, WorldState);
}
