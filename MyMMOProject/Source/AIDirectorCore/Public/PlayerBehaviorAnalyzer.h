#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIDirectorTypes.h"
#include "PlayerBehaviorAnalyzer.generated.h"

class UAIWorldDirectorSubsystem;
class UServerMetricsSubsystem;

UCLASS()
class AIDIRECTORCORE_API UPlayerBehaviorAnalyzer : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UAIWorldDirectorSubsystem* InDirector);
    void UpdateAnalysis();

    void RecordCombatData(const FUniqueNetIdRepl& PlayerID, float DPS, float TTK, float SurvivalTime, float DamageTaken);
    void RecordAbilityUse(const FUniqueNetIdRepl& PlayerID);
    void RecordDodge(const FUniqueNetIdRepl& PlayerID);
    void RecordBlock(const FUniqueNetIdRepl& PlayerID);

    float GetAverageDPS() const;
    float GetAverageTTK() const;
    float GetRangedUsage() const;
    float GetMeleeUsage() const;

private:
    UPROPERTY()
    UAIWorldDirectorSubsystem* Director;

    UPROPERTY()
    UServerMetricsSubsystem* Metrics;

    UPROPERTY()
    TMap<FUniqueNetIdRepl, FPlayerBehaviorData> PlayerData;

    void UpdateAggregates();
    void CleanupOldData();
    void EnforceMaxTrackedPlayers();

    static constexpr int32 MAX_TRACKED_PLAYERS = 2000;
    static constexpr int32 DATA_EXPIRATION_MINUTES = 30;

    float TotalDPS = 0.f;
    float TotalTTK = 0.f;
    int32 PlayerCount = 0;
    int32 RangedCount = 0;
    int32 MeleeCount = 0;
    int32 TotalDodges = 0;
    int32 TotalBlocks = 0;
    int32 TotalAbilities = 0;
};
