#pragma once

#include "CoreMinimal.h"
#include "AbilityInstance.h"
#include "CharacterEntity.h"
#include "GameConfig.h"
#include "ElementMatrix.h"

namespace CombatResolver
{
    float CalculateDamage(
        const FAbilityInstance& Ability,
        const UCharacterEntity* Caster,
        const UCharacterEntity* Target,
        const UElementMatrix* ElementMatrix,
        bool bIsPvP,
        const UGameConfig* Config,
        float FocusFireBonus = 1.0f
    );

    float CalculateDefenseReduction(float Defense);
    float CalculateResilienceReduction(float Resilience);
}
