#include "AdaptiveEnemyAI.h"
#include "AIWorldDirectorSubsystem.h"
#include "DifficultyGovernor.h"

void UAdaptiveEnemyAI::Initialize(UAIWorldDirectorSubsystem* InDirector)
{
    Director = InDirector;
    Governor = Director->GetDifficultyGovernor();
}

void UAdaptiveEnemyAI::AdaptMobs()
{
    float Efficiency = Director->GetCombatLearning()->GetGlobalEfficiency();
    float TargetAggression = 1.0f;
    float TargetHealth = 1.0f;
    float TargetDamage = 1.0f;

    if (Efficiency > 1.2f)
    {
        TargetAggression = FMath::Min(1.2f, Governor->GetMaxAggressionMultiplier());
        TargetHealth = FMath::Min(1.25f, Governor->GetMaxHealthMultiplier());
        TargetDamage = FMath::Min(1.15f, Governor->GetMaxDamageMultiplier());
    }
    else if (Efficiency < 0.8f)
    {
        TargetAggression = FMath::Max(0.8f, Governor->GetMinAggressionMultiplier());
        TargetHealth = FMath::Max(0.75f, Governor->GetMinHealthMultiplier());
        TargetDamage = FMath::Max(0.85f, Governor->GetMinDamageMultiplier());
    }

    for (auto& Pair : MobAdaptations)
    {
        float RandomFactor = FMath::FRandRange(RANDOM_FACTOR_MIN, RANDOM_FACTOR_MAX);
        Pair.Value.AggressionMultiplier = FMath::Lerp(
            Pair.Value.AggressionMultiplier,
            TargetAggression * RandomFactor,
            ADAPTATION_LERP_RATE
        );
        Pair.Value.HealthMultiplier = FMath::Lerp(
            Pair.Value.HealthMultiplier,
            TargetHealth * RandomFactor,
            ADAPTATION_LERP_RATE
        );
        Pair.Value.DamageMultiplier = FMath::Lerp(
            Pair.Value.DamageMultiplier,
            TargetDamage * RandomFactor,
            ADAPTATION_LERP_RATE
        );
    }
}

FMobAdaptation UAdaptiveEnemyAI::GetAdaptationForMob(int32 MobID) const
{
    if (const FMobAdaptation* Adapt = MobAdaptations.Find(MobID))
        return *Adapt;
    return FMobAdaptation();
}
