#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "AIDirectorTypes.h"
#include "AIWorldDirectorSubsystem.generated.h"

class UPlayerBehaviorAnalyzer;
class UCombatLearningSystem;
class UAdaptiveEnemyAI;
class UAdaptiveEliteSystem;
class UAdaptiveBossSystem;
class UDifficultyGovernor;
class UEconomyMonitor;
class UWorldEventOrchestrator;

UCLASS()
class AIDIRECTORCORE_API UAIWorldDirectorSubsystem
    : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UAIWorldDirectorSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

    UPlayerBehaviorAnalyzer* GetBehaviorAnalyzer() const { return BehaviorAnalyzer; }
    UCombatLearningSystem* GetCombatLearning() const { return CombatLearning; }
    UAdaptiveEnemyAI* GetAdaptiveEnemy() const { return AdaptiveEnemy; }
    UAdaptiveEliteSystem* GetAdaptiveElite() const { return AdaptiveElite; }
    UAdaptiveBossSystem* GetAdaptiveBoss() const { return AdaptiveBoss; }
    UDifficultyGovernor* GetDifficultyGovernor() const { return DifficultyGov; }
    UEconomyMonitor* GetEconomyMonitor() const { return EconomyMon; }
    UWorldEventOrchestrator* GetEventOrchestrator() const { return EventOrch; }

    const FServerLearningProfile& GetLearningProfile() const { return LearningProfile; }

private:
    bool IsServer() const;

    UPROPERTY()
    UPlayerBehaviorAnalyzer* BehaviorAnalyzer;

    UPROPERTY()
    UCombatLearningSystem* CombatLearning;

    UPROPERTY()
    UAdaptiveEnemyAI* AdaptiveEnemy;

    UPROPERTY()
    UAdaptiveEliteSystem* AdaptiveElite;

    UPROPERTY()
    UAdaptiveBossSystem* AdaptiveBoss;

    UPROPERTY()
    UDifficultyGovernor* DifficultyGov;

    UPROPERTY()
    UEconomyMonitor* EconomyMon;

    UPROPERTY()
    UWorldEventOrchestrator* EventOrch;

    FServerLearningProfile LearningProfile;

    // Таймеры
    FTimerHandle CombatAnalysisTimer;
    FTimerHandle EnemyAdaptTimer;
    FTimerHandle WorldEventTimer;
    FTimerHandle LearningUpdateTimer;

    void OnCombatAnalysis();
    void OnEnemyAdaptation();
    void OnWorldEvent();
    void OnLearningUpdate();
};
