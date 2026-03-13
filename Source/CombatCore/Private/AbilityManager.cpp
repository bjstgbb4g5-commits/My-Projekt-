#include "AbilityManager.h"
#include "Net/UnrealNetwork.h"

UAbilityManager::UAbilityManager()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UAbilityManager::AddAbility(const FAbilityInstance& Ability)
{
    Abilities.Add(Ability);
}

FAbilityInstance UAbilityManager::GetAbility(int32 SlotIndex) const
{
    if (SlotIndex >= 0 && SlotIndex < Abilities.Num())
        return Abilities[SlotIndex];
    return FAbilityInstance();
}

bool UAbilityManager::IsOnCooldown(int32 SlotIndex) const
{
    const float* Time = CooldownTimers.Find(SlotIndex);
    if (Time)
    {
        return GetWorld()->GetTimeSeconds() < *Time;
    }
    return false;
}

void UAbilityManager::PutOnCooldown(int32 SlotIndex)
{
    FAbilityInstance Ability = GetAbility(SlotIndex);
    if (Ability.BaseData)
    {
        CooldownTimers.Add(SlotIndex, GetWorld()->GetTimeSeconds() + Ability.GetCooldown());
    }
}

void UAbilityManager::TickCooldowns(float DeltaTime)
{
    TArray<int32> ToRemove;
    float CurrentTime = GetWorld()->GetTimeSeconds();
    for (auto& Pair : CooldownTimers)
    {
        if (CurrentTime >= Pair.Value)
        {
            ToRemove.Add(Pair.Key);
        }
    }
    for (int32 Slot : ToRemove)
    {
        CooldownTimers.Remove(Slot);
    }
}
