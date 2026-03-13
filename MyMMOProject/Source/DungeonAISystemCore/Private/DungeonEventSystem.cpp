#include "DungeonEventSystem.h"

EDungeonEventType UDungeonEventSystem::ChooseEvent()
{
    if (FMath::FRand() < 0.3f)
    {
        float Roll = FMath::FRand();
        if (Roll < 0.4f) return EDungeonEventType::Ambush;
        else if (Roll < 0.7f) return EDungeonEventType::ElitePatrol;
        else if (Roll < 0.9f) return EDungeonEventType::TreasureGoblin;
        else return EDungeonEventType::HiddenBoss;
    }
    return EDungeonEventType::None;
}

void UDungeonEventSystem::EvaluateEvents()
{
    // Заглушка: проход по активным данжам и выбор события
}

void UDungeonEventSystem::SpawnAmbush()
{
    // заглушка
}

void UDungeonEventSystem::SpawnElitePatrol()
{
    // заглушка
}

void UDungeonEventSystem::SpawnTreasureGoblin()
{
    // заглушка
}
