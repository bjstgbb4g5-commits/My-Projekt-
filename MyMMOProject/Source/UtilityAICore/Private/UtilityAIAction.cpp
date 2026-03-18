
#include "UtilityAIAction.h"
#include "NPCCharacter.h"

UUtilityAIAction::UUtilityAIAction()
{
}

float UUtilityAIAction::CalculateScore_Implementation(ANPCCharacter* NPC) const
{
    return BaseUtility;
}

void UUtilityAIAction::Execute_Implementation(ANPCCharacter* NPC)
{
}
