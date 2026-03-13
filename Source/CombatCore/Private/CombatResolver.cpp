#include "CombatResolver.h"
#include "Math/UnrealMathUtility.h"

float CombatResolver::CalculateDefenseReduction(float Defense)
{
    return Defense / (Defense + 500.f);
}

float CombatResolver::CalculateResilienceReduction(float Resilience)
{
    float EffectiveResilience = (Resilience <= 1000.f) ? Resilience : 1000.f + (Resilience - 1000.f) * 0.35f;
    return EffectiveResilience / (EffectiveResilience + 1000.f);
}

float CombatResolver::CalculateDamage(
    const FAbilityInstance& Ability,
    const UCharacterEntity* Caster,
    const UCharacterEntity* Target,
    const UElementMatrix* ElementMatrix,
    bool bIsPvP,
    const UGameConfig* Config,
    float FocusFireBonus)
{
    if (!Caster || !Target || !Config) return 0.f;

    float BaseDamage = Ability.GetPower() * Caster->Stats.Power;
    float ElementMod = ElementMatrix ? ElementMatrix->GetModifier(Ability.BaseData->Element, Target->Element) : 1.f;

    float CritRoll = FMath::FRand();
    float CritFactor = (CritRoll <= Caster->Stats.CritChance) ? Caster->Stats.CritMultiplier : 1.f;

    float DefenseReduction = CalculateDefenseReduction(Target->Stats.Defense);

    float Damage = BaseDamage * ElementMod * CritFactor * (1.f - DefenseReduction);

    if (bIsPvP)
    {
        Damage *= Config->PvPDamageModifier;
    }

    Damage *= FocusFireBonus;

    if (bIsPvP)
    {
        float ResilienceReduction = CalculateResilienceReduction(0.f); // TODO: добавить Resilience в CharacterEntity
        Damage *= (1.f - ResilienceReduction);
    }

    if (Ability.BaseData->bIsAoE)
    {
        Damage *= Config->AoEDamageMultiplier;
    }

    return FMath::Max(1.f, Damage);
}
