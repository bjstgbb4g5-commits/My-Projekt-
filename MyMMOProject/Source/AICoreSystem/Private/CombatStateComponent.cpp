#include "CombatStateComponent.h"
#include "Engine/World.h"

UCombatStateComponent::UCombatStateComponent()
{
    PrimaryComponentTick.bCanEverTick = false; // Управляется внешне
    CurrentState = ECombatState::Idle;
    LastStateChangeTime = 0.f;
}

void UCombatStateComponent::SetCombatState(ECombatState NewState)
{
    if (CurrentState == NewState) return;
    CurrentState = NewState;
    LastStateChangeTime = GetWorld()->GetTimeSeconds();
    OnCombatStateChanged.Broadcast(CurrentState);
}

void UCombatStateComponent::UpdateStateBasedOnDistance(float DistanceToTarget, float EngageDistance, float FightDistance)
{
    ECombatState NewState = CurrentState;

    if (DistanceToTarget <= FightDistance)
        NewState = ECombatState::Fighting;
    else if (DistanceToTarget <= EngageDistance)
        NewState = ECombatState::Engaging;
    else if (CurrentState == ECombatState::Fighting || CurrentState == ECombatState::Engaging)
        NewState = ECombatState::Retreating;
    else
        NewState = ECombatState::Idle;

    SetCombatState(NewState);
}

float UCombatStateComponent::GetTickInterval() const
{
    // В зависимости от состояния можно снижать частоту тиков для оптимизации
    switch (CurrentState)
    {
    case ECombatState::Fighting: return 0.1f;   // 10 раз в секунду
    case ECombatState::Engaging: return 0.2f;
    case ECombatState::Retreating: return 0.3f;
    default: return 0.5f; // Idle
    }
}
