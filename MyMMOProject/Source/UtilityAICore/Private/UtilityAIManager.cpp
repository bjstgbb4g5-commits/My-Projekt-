
#include "UtilityAIManager.h"
#include "UtilityAIComponent.h"

void UUtilityAIManager::Initialize(UWorld* InWorld)
{
    World = InWorld;
}

void UUtilityAIManager::Shutdown()
{
    NPCList.Empty();
}

void UUtilityAIManager::RegisterNPC(AActor* NPC)
{
    if (NPC)
    {
        NPCList.AddUnique(NPC);
    }
}

void UUtilityAIManager::UnregisterNPC(AActor* NPC)
{
    NPCList.Remove(NPC);
}

void UUtilityAIManager::UpdateAllAI()
{
    for (TWeakObjectPtr<AActor> NPCPtr : NPCList)
    {
        if (!NPCPtr.IsValid()) continue;

        AActor* NPC = NPCPtr.Get();
        UUtilityAIComponent* AIComp = NPC->FindComponentByClass<UUtilityAIComponent>();
        if (AIComp)
        {
            AIComp->EvaluateAI();
        }
    }
}
