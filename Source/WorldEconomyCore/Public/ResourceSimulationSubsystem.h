#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EconomyTypes.h"
#include "ResourceSimulationSubsystem.generated.h"

UCLASS()
class WORLDECONOMYCORE_API UResourceSimulationSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UResourceSimulationSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Economy|Resources")
    void SimulateResourceMigration();

    static constexpr int32 MAX_RESOURCE_NODES = 2000;

private:
    bool IsServer() const;
    void ScheduleSimulation(float IntervalSeconds);
    void OnSimulationTimer();

    FTimerHandle SimulationTimerHandle;

    UPROPERTY()
    TArray<FResourceNode> ResourceNodes;

    int32 NextNodeID = 1;

    void CreateResourceNode(int32 RegionID, EResourceType Type, int32 Tier);
    void RemoveResourceNode(int32 NodeID);
};
