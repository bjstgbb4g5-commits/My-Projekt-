#include "DungeonAISubsystem.h"
#include "DungeonDirector.h"
#include "EncounterGenerator.h"
#include "MobCompositionAI.h"
#include "AdaptiveBossMechanics.h"
#include "DungeonDifficultyBalancer.h"
#include "DungeonEventSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UDungeonAISubsystem::UDungeonAISubsystem()
{
}

void UDungeonAISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (!IsServer()) return;

    Director = NewObject<UDungeonDirector>(this);
    EncounterGen = NewObject<UEncounterGenerator>(this);
    CompositionAI = NewObject<UMobCompositionAI>(this);
    BossAI = NewObject<UAdaptiveBossMechanics>(this);
    DifficultyBalancer = NewObject<UDungeonDifficultyBalancer>(this);
    EventSystem = NewObject<UDungeonEventSystem>(this);

    UWorld* World = GetWorld();
    if (World)
    {
        World->GetTimerManager().SetTimer(AnalysisTimer, this, &UDungeonAISubsystem::OnGroupAnalysis, 5.0f, true);
        World->GetTimerManager().SetTimer(EncounterTimer, this, &UDungeonAISubsystem::OnEncounterUpdate, 20.0f, true);
        World->GetTimerManager().SetTimer(EventTimer, this, &UDungeonAISubsystem::OnEventCheck, 60.0f, true);
    }
}

void UDungeonAISubsystem::Deinitialize()
{
    UWorld* World = GetWorld();
    if (World)
    {
        World->GetTimerManager().ClearTimer(AnalysisTimer);
        World->GetTimerManager().ClearTimer(EncounterTimer);
        World->GetTimerManager().ClearTimer(EventTimer);
    }
    Super::Deinitialize();
}

bool UDungeonAISubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UDungeonAISubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UDungeonAISubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UDungeonAISubsystem, STATGROUP_Tickables);
}

void UDungeonAISubsystem::Tick(float DeltaTime)
{
    // Ничего не делаем, всё по таймерам
}

void UDungeonAISubsystem::OnGroupAnalysis()
{
    Director->AnalyzeGroup();
}

void UDungeonAISubsystem::OnEncounterUpdate()
{
    EncounterGen->UpdateEncounters();
}

void UDungeonAISubsystem::OnEventCheck()
{
    EventSystem->EvaluateEvents();
}
