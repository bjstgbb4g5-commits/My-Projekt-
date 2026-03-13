#include "BossPhaseComponent.h"
#include "Net/UnrealNetwork.h"

UBossPhaseComponent::UBossPhaseComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UBossPhaseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UBossPhaseComponent, CurrentPhase);
}

void UBossPhaseComponent::UpdatePhase(float HealthPercent)
{
    if (!GetOwner()->HasAuthority()) return;

    EBossPhase NewPhase = CurrentPhase;
    if (HealthPercent < 0.1f)
        NewPhase = EBossPhase::Berserk;
    else if (HealthPercent < 0.4f)
        NewPhase = EBossPhase::Phase3;
    else if (HealthPercent < 0.75f)
        NewPhase = EBossPhase::Phase2;
    else
        NewPhase = EBossPhase::Phase1;

    if (NewPhase != CurrentPhase)
    {
        CurrentPhase = NewPhase;
        OnPhaseChanged.Broadcast(CurrentPhase);
    }
}

void UBossPhaseComponent::OnRep_CurrentPhase()
{
    OnPhaseChanged.Broadcast(CurrentPhase);
}
