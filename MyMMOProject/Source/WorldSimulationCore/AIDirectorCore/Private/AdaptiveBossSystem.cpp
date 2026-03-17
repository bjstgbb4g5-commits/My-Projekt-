#include "AdaptiveBossSystem.h"
#include "AIWorldDirectorSubsystem.h"

void UAdaptiveBossSystem::Initialize(UAIWorldDirectorSubsystem* InDirector)
{
    Director = InDirector;
}

void UAdaptiveBossSystem::AdaptBosses()
{
    // Заглушка: выбор способностей в зависимости от тактик игроков
}

TArray<int32> UAdaptiveBossSystem::GetActiveAbilities(int32 BossID) const
{
    if (const TArray<int32>* Abilities = ActiveAbilities.Find(BossID))
        return *Abilities;
    return TArray<int32>();
}
