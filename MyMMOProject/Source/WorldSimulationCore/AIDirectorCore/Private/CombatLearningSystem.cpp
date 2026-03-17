#include "CombatLearningSystem.h"
#include "AIWorldDirectorSubsystem.h"
#include "PlayerBehaviorAnalyzer.h"

void UCombatLearningSystem::Initialize(UAIWorldDirectorSubsystem* InDirector)
{
    Director = InDirector;
    Analyzer = Director->GetBehaviorAnalyzer();
}

void UCombatLearningSystem::UpdateEfficiency()
{
    // Пока заглушка: вычисляем эффективность на основе средних показателей
    float AvgDPS = Analyzer->GetAverageDPS();
    float AvgTTK = Analyzer->GetAverageTTK();
    float AvgSurvival = 30.f; // взять из метрик
    float AvgDamageTaken = 100.f;

    if (AvgTTK <= 0.f) return;

    float MeasuredEfficiency = (AvgDPS * AvgSurvival) / (AvgTTK + AvgDamageTaken);
    MeasuredEfficiency = FMath::Clamp(MeasuredEfficiency, 0.5f, 2.0f);
    UpdateGlobalEfficiency(MeasuredEfficiency);
}

void UCombatLearningSystem::UpdateGlobalEfficiency(float MeasuredEfficiency)
{
    GlobalEfficiency = GlobalEfficiency * (1 - SMOOTHING_FACTOR) + MeasuredEfficiency * SMOOTHING_FACTOR;
}

float UCombatLearningSystem::GetPlayerEfficiency(const FUniqueNetIdRepl& PlayerID) const
{
    if (const FPlayerEfficiency* Eff = PlayerEfficiency.Find(PlayerID))
        return Eff->Efficiency;
    return 0.f;
}
