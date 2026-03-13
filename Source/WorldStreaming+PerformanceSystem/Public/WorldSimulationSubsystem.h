#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PerformanceTypes.h"
#include "WorldSimulationSubsystem.generated.h"

UCLASS()
class WORLDPERFORMANCECORE_API UWorldSimulationSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UWorldSimulationSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    const FServerMetrics& GetMetrics() const { return Metrics; }

private:
    bool IsServer() const;
    void SimulationTick();
    void UpdateAggro();
    void UpdateStamina();

    FTSTicker::FDelegateHandle TickHandle;
    bool Tick(float DeltaTime);

    FServerMetrics Metrics;
    TMap<FUniqueNetIdRepl, float> LastActionTimes; // для anti-spam
};
