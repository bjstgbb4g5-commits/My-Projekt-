#include "HouseComponent.h"
#include "Net/UnrealNetwork.h"

UHouseComponent::UHouseComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
    HouseId = -1;
    Rank = EHouseRank::Member;
}

void UHouseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UHouseComponent, HouseId, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UHouseComponent, Rank, COND_OwnerOnly);
}

void UHouseComponent::SetHouse(int32 InHouseId, EHouseRank InRank)
{
    if (!GetOwner()->HasAuthority()) return;
    HouseId = InHouseId;
    Rank = InRank;
}
