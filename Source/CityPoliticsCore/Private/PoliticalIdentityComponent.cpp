#include "PoliticalIdentityComponent.h"
#include "Net/UnrealNetwork.h"

UPoliticalIdentityComponent::UPoliticalIdentityComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);

    PoliticalRank = EPoliticalRank::Citizen;
    ConsecutiveMayorTerms = 0;
    RegisteredCityId = 0;
}

void UPoliticalIdentityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UPoliticalIdentityComponent, PoliticalRank);
    DOREPLIFETIME(UPoliticalIdentityComponent, ConsecutiveMayorTerms);
    DOREPLIFETIME(UPoliticalIdentityComponent, RegisteredCityId);
}
