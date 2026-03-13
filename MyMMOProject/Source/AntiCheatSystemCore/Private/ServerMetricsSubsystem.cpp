#include "ServerMetricsSubsystem.h"
#include "Engine/World.h"

UServerMetricsSubsystem::UServerMetricsSubsystem()
{
}

void UServerMetricsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UServerMetricsSubsystem::Deinitialize()
{
    Metrics.Empty();
    DamageAccumulator.Empty();
    GoldAccumulator.Empty();
    KillAccumulator.Empty();
    ActionAccumulator.Empty();
    Super::Deinitialize();
}

bool UServerMetricsSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UServerMetricsSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UServerMetricsSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UServerMetricsSubsystem, STATGROUP_Tickables);
}

void UServerMetricsSubsystem::Tick(float DeltaTime)
{
    MetricsTimer += DeltaTime;
    if (MetricsTimer >= METRICS_UPDATE_INTERVAL)
    {
        MetricsTimer = 0.f;
        UpdateMetrics();
    }
}

void UServerMetricsSubsystem::RecordDamage(const FUniqueNetIdRepl& PlayerID, int32 Damage)
{
    DamageAccumulator.FindOrAdd(PlayerID) += Damage;
}

void UServerMetricsSubsystem::RecordGold(const FUniqueNetIdRepl& PlayerID, int32 Gold)
{
    GoldAccumulator.FindOrAdd(PlayerID) += Gold;
}

void UServerMetricsSubsystem::RecordKill(const FUniqueNetIdRepl& PlayerID)
{
    KillAccumulator.FindOrAdd(PlayerID)++;
}

void UServerMetricsSubsystem::RecordAction(const FUniqueNetIdRepl& PlayerID)
{
    ActionAccumulator.FindOrAdd(PlayerID)++;
}

void UServerMetricsSubsystem::UpdateMetrics()
{
    FDateTime Now = FDateTime::UtcNow();

    for (auto& Pair : DamageAccumulator)
    {
        FPlayerMetrics& Metric = Metrics.FindOrAdd(Pair.Key);
        Metric.PlayerID = Pair.Key;
        Metric.DamagePerMinute = Pair.Value;
        Metric.LastUpdate = Now;
        Pair.Value = 0;
    }
    for (auto& Pair : GoldAccumulator)
    {
        FPlayerMetrics& Metric = Metrics.FindOrAdd(Pair.Key);
        Metric.GoldPerMinute = Pair.Value;
        Pair.Value = 0;
    }
    for (auto& Pair : KillAccumulator)
    {
        FPlayerMetrics& Metric = Metrics.FindOrAdd(Pair.Key);
        Metric.KillsPerMinute = Pair.Value;
        Pair.Value = 0;
    }
    for (auto& Pair : ActionAccumulator)
    {
        FPlayerMetrics& Metric = Metrics.FindOrAdd(Pair.Key);
        Metric.ActionsPerMinute = Pair.Value;
        Pair.Value = 0;
    }
}

const FPlayerMetrics* UServerMetricsSubsystem::GetPlayerMetrics(const FUniqueNetIdRepl& PlayerID) const
{
    return Metrics.Find(PlayerID);
}
