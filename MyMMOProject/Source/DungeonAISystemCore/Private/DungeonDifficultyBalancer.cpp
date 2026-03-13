#include "DungeonDifficultyBalancer.h"

float UDungeonDifficultyBalancer::GetHealthMultiplier(const FDungeonGroupProfile& Profile)
{
    float Base = 1.0f;
    if (Profile.GroupDPS > 5000.f) Base += 0.2f;
    if (Profile.Deaths < 1) Base += 0.1f;
    return FMath::Clamp(Base, 0.5f, 1.5f);
}

float UDungeonDifficultyBalancer::GetDamageMultiplier(const FDungeonGroupProfile& Profile)
{
    float Base = 1.0f;
    if (Profile.HealRate > 300.f) Base += 0.15f;
    if (Profile.DodgeFrequency > 0.7f) Base += 0.1f;
    return FMath::Clamp(Base, 0.5f, 1.5f);
}
