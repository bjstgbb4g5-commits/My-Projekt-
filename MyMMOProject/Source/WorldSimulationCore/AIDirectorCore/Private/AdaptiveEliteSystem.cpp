#include "AdaptiveEliteSystem.h"
#include "AIWorldDirectorSubsystem.h"
#include "PlayerBehaviorAnalyzer.h"

void UAdaptiveEliteSystem::Initialize(UAIWorldDirectorSubsystem* InDirector)
{
    Director = InDirector;
}

void UAdaptiveEliteSystem::AdaptElites()
{
    float RangedUsage = Director->GetBehaviorAnalyzer()->GetRangedUsage();
    for (auto& Pair : EliteModules)
    {
        TArray<EEliteModule>& Modules = Pair.Value;
        // Пример: если много дальников, добавляем Hunter
        if (RangedUsage > 0.6f && !Modules.Contains(EEliteModule::Hunter))
            Modules.Add(EEliteModule::Hunter);
        // и т.д.
    }
}

TArray<EEliteModule> UAdaptiveEliteSystem::ChooseModulesForElite(int32 EliteID) const
{
    if (const TArray<EEliteModule>* Modules = EliteModules.Find(EliteID))
        return *Modules;
    return TArray<EEliteModule>();
}
