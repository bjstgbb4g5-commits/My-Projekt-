
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrowdTypes.h"
#include "CrowdAgentComponent.generated.h"

UCLASS(ClassGroup=(NPC), meta=(BlueprintSpawnableComponent))
class CROWDSIMULATIONCORE_API UCrowdAgentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCrowdAgentComponent();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void SetDesiredVelocity(const FVector& Velocity);
    FVector GetDesiredVelocity() const { return AgentData.DesiredVelocity; }

    void SetCurrentVelocity(const FVector& Velocity);
    FVector GetCurrentVelocity() const { return AgentData.CurrentVelocity; }

    void SetAgentRadius(float NewRadius) { AgentData.Radius = NewRadius; }
    float GetAgentRadius() const { return AgentData.Radius; }

    void SetMaxSpeed(float NewMaxSpeed) { AgentData.MaxSpeed = NewMaxSpeed; }
    float GetMaxSpeed() const { return AgentData.MaxSpeed; }

    void SetAgentState(ECrowdAgentState NewState) { AgentData.State = NewState; }
    ECrowdAgentState GetAgentState() const { return AgentData.State; }

    FVector GetAgentLocation() const;
    const FCrowdAgentData& GetAgentData() const { return AgentData; }
    FCrowdAgentData& GetAgentData() { return AgentData; }

private:
    FCrowdAgentData AgentData;
};
