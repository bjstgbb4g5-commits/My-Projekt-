#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "ServerMetricsTypes.h"
#include "ServerMetricsSubsystem.generated.h"

UCLASS()
class ANTICHEATSYSTEMCORE_API UServerMetricsSubsystem
    : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UServerMetricsSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

    void RecordDamage(const FUniqueNetIdRepl& PlayerID, int32 Damage);
    void RecordGold(const FUniqueNetIdRepl& PlayerID, int32 Gold);
    void RecordKill(const FUniqueNetIdRepl& PlayerID);
    void RecordAction(const FUniqueNetIdRepl& PlayerID);

    const FPlayerMetrics* GetPlayerMetrics(const FUniqueNetIdRepl& PlayerID) const;

private:
    bool IsServer() const;
    void UpdateMetrics();

    UPROPERTY()
    TMap<FUniqueNetIdRepl, FPlayerMetrics> Metrics;

    float MetricsTimer = 0.f;
    static constexpr float METRICS_UPDATE_INTERVAL = 60.f;

    // Аккумуляторы для расчёта per minute
    TMap<FUniqueNetIdRepl, int32> DamageAccumulator;
    TMap<FUniqueNetIdRepl, int32> GoldAccumulator;
    TMap<FUniqueNetIdRepl, int32> KillAccumulator;
    TMap<FUniqueNetIdRepl, int32> ActionAccumulator;
};
