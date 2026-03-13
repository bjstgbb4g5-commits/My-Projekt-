#include "AILODComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "AIController.h"
#include "BrainComponent.h"

UAILODComponent::UAILODComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentLOD = EAILODLevel::Active;
    bInCombat = false;
}

void UAILODComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UAILODComponent::SetInCombat(bool bCombat)
{
    bInCombat = bCombat;
}

void UAILODComponent::UpdateLOD(float Distance)
{
    if (bInCombat)
    {
        SetLOD(EAILODLevel::Active);
        return;
    }

    EAILODLevel NewLOD = EAILODLevel::Sleeping;
    if (Distance <= ActiveDistance) NewLOD = EAILODLevel::Active;
    else if (Distance <= ReducedDistance) NewLOD = EAILODLevel::Reduced;
    else NewLOD = EAILODLevel::Sleeping;

    SetLOD(NewLOD);
}

void UAILODComponent::SetLOD(EAILODLevel NewLOD)
{
    if (CurrentLOD == NewLOD) return;
    CurrentLOD = NewLOD;

    AAIController* AIController = Cast<AAIController>(GetOwner()->GetInstigatorController());
    if (!AIController) return;
    UBrainComponent* Brain = AIController->GetBrainComponent();

    switch (CurrentLOD)
    {
    case EAILODLevel::Active:
        AIController->SetActorTickEnabled(true);
        if (Brain) Brain->RestartLogic();
        AIController->SetActorTickInterval(0.f);
        break;
    case EAILODLevel::Reduced:
        AIController->SetActorTickEnabled(true);
        if (Brain) Brain->RestartLogic();
        AIController->SetActorTickInterval(0.5f);
        break;
    case EAILODLevel::Sleeping:
        if (Brain) Brain->StopLogic(TEXT("Sleeping"));
        AIController->SetActorTickEnabled(false);
        break;
    }
}

float UAILODComponent::GetDistanceToNearestPlayer() const
{
    float MinDist = FLT_MAX;
    UWorld* World = GetWorld();
    if (!World) return MinDist;
    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->GetPawn())
        {
            float Dist = FVector::Dist(GetOwner()->GetActorLocation(), PC->GetPawn()->GetActorLocation());
            if (Dist < MinDist) MinDist = Dist;
        }
    }
    return MinDist;
}
