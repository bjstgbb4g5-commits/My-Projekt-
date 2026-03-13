#include "PartyComponent.h"
#include "Net/UnrealNetwork.h"

UPartyComponent::UPartyComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
    PartyId = -1;
    Role = EPartyRole::Member;
}

void UPartyComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UPartyComponent, PartyId, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UPartyComponent, Role, COND_OwnerOnly);
}

void UPartyComponent::SetParty(int32 InPartyId, EPartyRole InRole)
{
    if (!GetOwner()->HasAuthority()) return;
    PartyId = InPartyId;
    Role = InRole;
}
