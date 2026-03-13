#include "ResourceWorldState.h"
#include "Net/UnrealNetwork.h"

AResourceWorldState::AResourceWorldState()
{
    bReplicates = true;
    bAlwaysRelevant = true;
}

void AResourceWorldState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AResourceWorldState, ResourceNodes);
}

void AResourceWorldState::OnRep_ResourceNodes()
{
    // Клиент может обновить UI
}
