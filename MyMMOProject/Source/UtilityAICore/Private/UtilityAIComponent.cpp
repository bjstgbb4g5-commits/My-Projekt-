
#include "UtilityAIComponent.h"
#include "UtilityAIAction.h"
#include "NPCCharacter.h"
#include "Engine/World.h"

UUtilityAIComponent::UUtilityAIComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UUtilityAIComponent::BeginPlay()
{
    Super::BeginPlay();
    LastEvaluationTime = GetWorld()->GetTimeSeconds();
}

ANPCCharacter* UUtilityAIComponent::GetNPCCharacter() const
{
    return Cast<ANPCCharacter>(GetOwner());
}

void UUtilityAIComponent::AddAction(TSubclassOf<UUtilityAIAction> ActionClass)
{
    if (!ActionClass) return;

    UUtilityAIAction* NewAction = NewObject<UUtilityAIAction>(this, ActionClass);
    if (NewAction)
    {
        Actions.Add(NewAction);
    }
}

void UUtilityAIComponent::EvaluateAI()
{
    ANPCCharacter* NPC = GetNPCCharacter();
    if (!NPC) return;

    float BestScore = -1.0f;
    UUtilityAIAction* BestAction = nullptr;
    float CurrentTime = GetWorld()->GetTimeSeconds();

    for (UUtilityAIAction* Action : Actions)
    {
        if (!Action) continue;

        if (CurrentTime - Action->LastExecuteTime < Action->CooldownTime)
        {
            continue;
        }

        float Score = Action->CalculateScore(NPC);
        if (Score > BestScore)
        {
            BestScore = Score;
            BestAction = Action;
        }
    }

    if (BestAction)
    {
        CurrentAction = BestAction->GetActionType();
        BestAction->Execute(NPC);
        BestAction->LastExecuteTime = CurrentTime;
    }
}
