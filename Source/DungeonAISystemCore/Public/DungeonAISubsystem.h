#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "DungeonAITypes.h"
#include "DungeonAISubsystem.generated.h"

class UDungeonDirector;
class UEncounterGenerator;
class UMobCompositionAI;
class UAdaptiveBossMechanics;
class UDungeonDifficultyBalancer;
class UDungeonEventSystem;

UCLASS()
class DUNGEONAISYSTEMCORE_API UDungeonAISubsystem
    : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UDungeonAISubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

    UDungeonDirector* GetDungeonDirector() const { return Director; }

    static constexpr int32 MAX_DUNGEONS = 100;
    static constexpr int32 MAX_MOBS_PER_DUNGEON = 120;
    static constexpr int32 MAX_ELITES = 10;
    static constexpr int32 MAX_EVENTS = 4;

private:
    bool IsServer() const;

    UPROPERTY()
    UDungeonDirector* Director;

    UPROPERTY()
    UEncounterGenerator* EncounterGen;

    UPROPERTY()
    UMobCompositionAI* CompositionAI;

    UPROPERTY()
    UAdaptiveBossMechanics* BossAI;

    UPROPERTY()
    UDungeonDifficultyBalancer* DifficultyBalancer;

    UPROPERTY()
    UDungeonEventSystem* EventSystem;

    FTimerHandle AnalysisTimer;
    FTimerHandle EncounterTimer;
    FTimerHandle EventTimer;

    void OnGroupAnalysis();
    void OnEncounterUpdate();
    void OnEventCheck();
};
