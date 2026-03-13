#include "AIWorldDirectorSubsystem.h"
#include "PlayerBehaviorAnalyzer.h"
#include "CombatLearningSystem.h"
#include "AdaptiveEnemyAI.h"
#include "AdaptiveEliteSystem.h"
#include "AdaptiveBossSystem.h"
#include "DifficultyGovernor.h"
#include "EconomyMonitor.h"
#include "WorldEventOrchestrator.h"
#include "Engine/World.h"
#include "TimerManager.h"

UAIWorldDirectorSubsystem::UAIWorldDirectorSubsystem()
{
}

void UAIWorldDirectorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (!IsServer()) return;

    BehaviorAnalyzer = NewObject<UPlayerBehaviorAnalyzer>(this);
    CombatLearning = NewObject<UCombatLearningSystem>(this);
    AdaptiveEnemy = NewObject<UAdaptiveEnemyAI>(this);
    AdaptiveElite = NewObject<UAdaptiveEliteSystem>(this);
    AdaptiveBoss = NewObject<UAdaptiveBossSystem>(this);
    DifficultyGov = NewObject<UDifficultyGovernor>(this);
    EconomyMon = NewObject<UEconomyMonitor>(this);
    EventOrch = NewObject<UWorldEventOrchestrator>(this);

    BehaviorAnalyzer->Initialize(this);
    CombatLearning->Initialize(this);
    AdaptiveEnemy->Initialize(this);
    AdaptiveElite->Initialize(this);
    AdaptiveBoss->Initialize(this);
    DifficultyGov->Initialize(this);
    EconomyMon->Initialize(this);
    EventOrch->Initialize(this);

    UWorld* World = GetWorld();
    if (World)
    {
        World->GetTimerManager().SetTimer(CombatAnalysisTimer, this, &UAIWorldDirectorSubsystem::OnCombatAnalysis, 10.0f, true);
        World->GetTimerManager().SetTimer(EnemyAdaptTimer, this, &UAIWorldDirectorSubsystem::OnEnemyAdaptation, 30.0f, true);
        World->GetTimerManager().SetTimer(WorldEventTimer, this, &UAIWorldDirectorSubsystem::OnWorldEvent, 300.0f, true);
        World->GetTimerManager().SetTimer(LearningUpdateTimer, this, &UAIWorldDirectorSubsystem::OnLearningUpdate, 600.0f, true);
    }
}

void UAIWorldDirectorSubsystem::Deinitialize()
{
    UWorld* World = GetWorld();
    if (World)
    {
        World->GetTimerManager().ClearTimer(CombatAnalysisTimer);
        World->GetTimerManager().ClearTimer(EnemyAdaptTimer);
        World->GetTimerManager().ClearTimer(WorldEventTimer);
        World->GetTimerManager().ClearTimer(LearningUpdateTimer);
    }
    Super::Deinitialize();
}

bool UAIWorldDirectorSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UAIWorldDirectorSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UAIWorldDirectorSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UAIWorldDirectorSubsystem, STATGROUP_Tickables);
}

void UAIWorldDirectorSubsystem::Tick(float DeltaTime)
{
    // Ничего не делаем, всё по таймерам
}

void UAIWorldDirectorSubsystem::OnCombatAnalysis()
{
    BehaviorAnalyzer->UpdateAnalysis();
    CombatLearning->UpdateEfficiency();
}

void UAIWorldDirectorSubsystem::OnEnemyAdaptation()
{
    AdaptiveEnemy->AdaptMobs();
    AdaptiveElite->AdaptElites();
    AdaptiveBoss->AdaptBosses();
}

void UAIWorldDirectorSubsystem::OnWorldEvent()
{
    EventOrch->ConsiderEvents();
}

void UAIWorldDirectorSubsystem::OnLearningUpdate()
{
    // Обновляем профиль сервера (будет реализовано позже)
}
