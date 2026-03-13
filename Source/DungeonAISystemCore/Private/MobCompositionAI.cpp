#include "MobCompositionAI.h"

EDungeonMobRole UMobCompositionAI::ChooseMobRole(const FDungeonGroupProfile& Profile)
{
    if (Profile.DodgeFrequency > 0.7f)
        return EDungeonMobRole::Assassin;
    if (Profile.GroupDPS > 5000.f)
        return EDungeonMobRole::Tank;
    if (Profile.HealRate > 300.f)
        return EDungeonMobRole::Support;
    return EDungeonMobRole::Melee;
}
