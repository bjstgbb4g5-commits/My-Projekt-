#include "AdaptiveBossMechanics.h"

TArray<int32> UAdaptiveBossMechanics::ChooseBossAbilities(int32 BossID, const FDungeonGroupProfile& Profile)
{
    TArray<int32> Result;
    Result.Add(1001); // базовая атака
    Result.Add(1002); // базовая способность

    if (Profile.GroupDPS > 4000.f)
        Result.Add(2001); // AOE
    if (Profile.HealRate > 200.f)
        Result.Add(2002); // анти-хил
    if (Profile.DodgeFrequency > 0.6f)
        Result.Add(2003); // неуловимая атака

    return Result;
}
