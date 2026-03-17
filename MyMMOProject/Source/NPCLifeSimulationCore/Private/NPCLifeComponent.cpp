#include "NPCLifeComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

UNPCLifeComponent::UNPCLifeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UNPCLifeComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UNPCLifeComponent::InitializeFromData(const FNPCLifeData& InData)
{
    LifeData = InData;
    CurrentTask = ENPCDailyTask::Idle;
}

void UNPCLifeComponent::UpdateLife(float CurrentHour)
{
    if (LODLevel >= 2) return;

    for (const FNPCScheduleEntry& Entry : LifeData.Schedule)
    {
        if (CurrentHour >= Entry.StartHour && CurrentHour < Entry.StartHour + 1.0f)
        {
            SetCurrentTask(Entry.Task, Entry.TargetLocation);
            break;
        }
    }
}

void UNPCLifeComponent::SetCurrentTask(ENPCDailyTask NewTask, AActor* TargetLocation)
{
    CurrentTask = NewTask;
    CurrentTarget = TargetLocation;
    TaskStartTime = GetWorld()->GetTimeSeconds();

    AAIController* AIController = Cast<AAIController>(GetOwner()->GetInstigatorController());
    if (AIController)
    {
        AIController->StopMovement();
    }
}

void UNPCLifeComponent::PerformTask(float DeltaTime)
{
    if (LODLevel >= 1) return;

    if (CurrentTask == ENPCDailyTask::Travel && CurrentTarget)
    {
        AAIController* AIController = Cast<AAIController>(GetOwner()->GetInstigatorController());
        if (AIController)
        {
            UPathFollowingComponent* PathFollowing = AIController->GetPathFollowingComponent();
            if (PathFollowing && PathFollowing->GetStatus() != EPathFollowingStatus::Moving)
            {
                AIController->MoveToActor(CurrentTarget, 100.0f);
            }
        }
    }
}
