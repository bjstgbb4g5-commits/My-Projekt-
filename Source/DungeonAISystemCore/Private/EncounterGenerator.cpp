#include "EncounterGenerator.h"

FDungeonEncounter UEncounterGenerator::GenerateEncounter(const FDungeonGroupProfile& Profile)
{
    FDungeonEncounter Encounter;
    int32 MobCount = BASE_MOBS;
    if (Profile.GroupDPS > 3000.f) MobCount += 2;
    if (Profile.Deaths > 3) MobCount -= 1;

    for (int32 i = 0; i < MobCount; ++i)
        Encounter.MobIDs.Add(FMath::RandRange(1000, 2000));

    if (Profile.GroupDPS > 4000.f && Profile.Deaths < 2)
        Encounter.EliteCount = 1;

    return Encounter;
}

void UEncounterGenerator::UpdateEncounters()
{
    // Заглушка: перегенерация энкаунтеров для активных данжей
}
