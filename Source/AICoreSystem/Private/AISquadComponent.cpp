#include "AISquadComponent.h"
#include "Net/UnrealNetwork.h"

UAISquadComponent::UAISquadComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UAISquadComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UAISquadComponent, Leader);
    DOREPLIFETIME(UAISquadComponent, SquadMembers);
    DOREPLIFETIME(UAISquadComponent, SquadRole);
}

void UAISquadComponent::SetLeader(AActor* NewLeader)
{
    if (!GetOwner()->HasAuthority()) return;
    Leader = NewLeader;
}

AActor* UAISquadComponent::GetLeader() const
{
    return Leader;
}

void UAISquadComponent::RegisterMember(AActor* Member)
{
    if (!GetOwner()->HasAuthority()) return;
    SquadMembers.AddUnique(Member);
}

void UAISquadComponent::UnregisterMember(AActor* Member)
{
    if (!GetOwner()->HasAuthority()) return;
    SquadMembers.Remove(Member);
}
