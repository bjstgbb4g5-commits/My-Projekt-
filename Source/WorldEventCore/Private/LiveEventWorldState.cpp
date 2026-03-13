#include "LiveEventWorldState.h"
#include "Net/UnrealNetwork.h"

ALiveEventWorldState::ALiveEventWorldState()
{
    bReplicates = true;
    bAlwaysRelevant = true;
}

void ALiveEventWorldState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ALiveEventWorldState, ActiveEvents);
}

void ALiveEventWorldState::OnRep_ActiveEvents()
{
    // Клиент может обновить UI
}
