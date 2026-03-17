#include "EconomySimulationSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UEconomySimulationSubsystem::UEconomySimulationSubsystem()
{
}

void UEconomySimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        ScheduleSimulation(300.0f); // каждые 5 минут
    }
}

void UEconomySimulationSubsystem::Deinitialize()
{
    if (IsServer())
    {
        GetWorld()->GetTimerManager().ClearTimer(SimulationTimerHandle);
    }
    Super::Deinitialize();
}

bool UEconomySimulationSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

void UEconomySimulationSubsystem::ScheduleSimulation(float IntervalSeconds)
{
    GetWorld()->GetTimerManager().SetTimer(SimulationTimerHandle, this, &UEconomySimulationSubsystem::OnSimulationTimer, IntervalSeconds, true);
}

void UEconomySimulationSubsystem::OnSimulationTimer()
{
    SimulateEconomy();
}

void UEconomySimulationSubsystem::SimulateEconomy()
{
    // Заглушка: сбор метрик
    Metrics.TotalGoldInWorld += 1000.f;
    Metrics.TotalItemsCrafted += 50;
    Metrics.TotalItemsDestroyed += 10;
}
