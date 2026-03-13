#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EconomyTypes.h"
#include "EconomySimulationSubsystem.generated.h"

UCLASS()
class ECONOMYCORE_API UEconomySimulationSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UEconomySimulationSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Economy|Simulation")
    void SimulateEconomy();

    UFUNCTION(BlueprintCallable, Category = "Economy|Metrics")
    FEconomyMetrics GetMetrics() const { return Metrics; }

private:
    bool IsServer() const;
    void ScheduleSimulation(float IntervalSeconds);
    void OnSimulationTimer();

    FTimerHandle SimulationTimerHandle;
    FEconomyMetrics Metrics;
};
