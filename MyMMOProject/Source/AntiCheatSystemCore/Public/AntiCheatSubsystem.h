#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "AntiCheatTypes.h"
#include "AntiCheatSubsystem.generated.h"

class UServerMetricsSubsystem;
class UBanManager;

UCLASS()
class ANTICHEATSYSTEMCORE_API UAntiCheatSubsystem
    : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UAntiCheatSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

    void ReportMovement(const FUniqueNetIdRepl& PlayerID, const FVector& Location, float Speed, float DeltaTime);
    void ReportDamage(const FUniqueNetIdRepl& PlayerID, float Damage, float MaxAbilityDamage);
    void ReportGoldGain(const FUniqueNetIdRepl& PlayerID, int32 Amount);
    void ReportAbilityUse(const FUniqueNetIdRepl& PlayerID);
    void ReportSuspiciousAction(const FUniqueNetIdRepl& PlayerID, ECheatType Type, int32 Score);
    void OnPlayerLogout(const FUniqueNetIdRepl& PlayerID);

private:
    bool IsServer() const;
    void AddSuspicion(const FUniqueNetIdRepl& PlayerID, ECheatType Type, int32 Score);
    void EvaluatePlayer(const FUniqueNetIdRepl& PlayerID);
    void ApplyPenalty(const FUniqueNetIdRepl& PlayerID, const FString& Reason);

    UPROPERTY()
    TMap<FUniqueNetIdRepl, FPlayerSuspicionData> SuspicionData;

    UPROPERTY()
    UServerMetricsSubsystem* Metrics;

    UPROPERTY()
    UBanManager* BanManager;

    static constexpr float MAX_ALLOWED_SPEED = 1200.f;
    static constexpr float DAMAGE_HACK_MULTIPLIER = 1.5f;
    static constexpr int32 MAX_GOLD_PER_MINUTE = 100000;
    static constexpr int32 MIN_ABILITY_INTERVAL_MS = 30;
    static constexpr int32 ABILITY_SPAM_THRESHOLD = 5;
    static constexpr int32 AUTOKICK_SCORE = 100;
    static constexpr int32 AUTOBAN_SCORE = 200;
    static constexpr float ECONOMY_SPIKE_WINDOW = 10.f;
    static constexpr int32 ECONOMY_SPIKE_LIMIT = 20000;
    static constexpr float DECAY_INTERVAL = 300.f;
    static constexpr int32 DECAY_AMOUNT = 5;

    struct FRecentAbilityData
    {
        TArray<FDateTime> RecentTimestamps;
        int32 Count = 0;
    };
    TMap<FUniqueNetIdRepl, FRecentAbilityData> AbilitySpamTracker;

    struct FEconomySpikeData
    {
        int32 TotalGain = 0;
        FDateTime StartTime;
    };
    TMap<FUniqueNetIdRepl, FEconomySpikeData> EconomySpikeTracker;

    float DecayAccumulator = 0.f;
};
