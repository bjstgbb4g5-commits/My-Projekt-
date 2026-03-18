
#include "CrowdAgentComponent.h"
#include "CrowdSubsystem.h"
#include "CrowdManager.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

UCrowdAgentComponent::UCrowdAgentComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCrowdAgentComponent::BeginPlay()
{
    Super::BeginPlay();

    if (UCrowdSubsystem* Subsystem = GetWorld()->GetSubsystem<UCrowdSubsystem>())
    {
        if (UCrowdManager* Manager = Subsystem->GetManager())
        {
            Manager->RegisterAgent(this);
        }
    }

    AgentData.Owner = GetOwner();
}

void UCrowdAgentComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UCrowdSubsystem* Subsystem = GetWorld()->GetSubsystem<UCrowdSubsystem>())
    {
        if (UCrowdManager* Manager = Subsystem->GetManager())
        {
            Manager->UnregisterAgent(this);
        }
    }

    Super::EndPlay(EndPlayReason);
}

void UCrowdAgentComponent::SetDesiredVelocity(const FVector& Velocity)
{
    AgentData.DesiredVelocity = Velocity;
}

void UCrowdAgentComponent::SetCurrentVelocity(const FVector& Velocity)
{
    AgentData.CurrentVelocity = Velocity;
}

FVector UCrowdAgentComponent::GetAgentLocation() const
{
    AActor* Owner = GetOwner();
    return Owner ? Owner->GetActorLocation() : FVector::ZeroVector;
}
