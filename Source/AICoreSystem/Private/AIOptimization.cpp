#include "AIOptimization.h"

void UOptimizationManager::Initialize(const FAIOptimizationSettings& Settings)
{
    CurrentSettings = Settings;
}

EOptimizationTier UOptimizationManager::GetTierForDistance(float Distance) const
{
    if (Distance <= CurrentSettings.FullDistance)
        return EOptimizationTier::Full;
    if (Distance <= CurrentSettings.ReducedDistance)
        return EOptimizationTier::Reduced;
    if (Distance <= CurrentSettings.MinimalDistance)
        return EOptimizationTier::Minimal;
    return EOptimizationTier::Off;
}
