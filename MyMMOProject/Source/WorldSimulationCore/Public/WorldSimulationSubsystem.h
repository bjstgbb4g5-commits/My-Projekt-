#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "Stats/Stats.h"
#include "WorldStateTypes.h"
#include "WorldTickManager.h"
#include "WorldSimulationSubsystem.generated.h"

class UWorldEconomySubsystem;
class UWorldEventSubsystem;
class UWorldPopulationSubsystem;
class UAIWorldDirectorSubsystem;

DECLARE_STATS_GROUP(TEXT("WorldSimulation"), STATGROUP_WorldSimulation, STATCAT_Advanced);

UCLASS()
class WORLDSIMULATIONCORE_API UWorldSimulationSubsystem
    : public UWorldSubsystem
    , public FTickableGameObject
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;
    virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
    virtual bool IsTickable() const override;

    void StartSimulation();
    void PauseSimulation();
    bool IsSimulationRunning() const { return bSimulationRunning; }

    const FWorldState& GetWorldState() const { return WorldState; }

private:
    void RunSimulationStep();

    void UpdateEconomy(float DeltaTime);
    void UpdatePopulation(float DeltaTime);
    void UpdateEvents(float DeltaTime);
    void UpdateAIDirector(float DeltaTime);

    bool bSimulationRunning = false;
    FWorldTickManager TickManager;
    FWorldState WorldState;

    UPROPERTY(Transient)
    UWorldEconomySubsystem* EconomySubsystem;

    UPROPERTY(Transient)
    UWorldEventSubsystem* EventSubsystem;

    UPROPERTY(Transient)
    UWorldPopulationSubsystem* PopulationSubsystem;

    UPROPERTY(Transient)
    UAIWorldDirectorSubsystem* AIDirectorSubsystem;
};
