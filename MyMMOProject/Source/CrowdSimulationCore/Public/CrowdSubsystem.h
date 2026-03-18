
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "Stats/Stats.h"
#include "CrowdManager.h"
#include "CrowdSubsystem.generated.h"

DECLARE_STATS_GROUP(TEXT("CrowdSimulation"), STATGROUP_CrowdSimulation, STATCAT_Advanced);

UCLASS()
class CROWDSIMULATIONCORE_API UCrowdSubsystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;
    virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
    virtual bool IsTickable() const override;

    UCrowdManager* GetManager() const { return Manager; }

private:
    UPROPERTY(Transient)
    UCrowdManager* Manager;

    float CurrentTime = 0.0f;
    float UpdateTimer = 0.0f;
    static constexpr float UPDATE_INTERVAL = 0.1f;
};
