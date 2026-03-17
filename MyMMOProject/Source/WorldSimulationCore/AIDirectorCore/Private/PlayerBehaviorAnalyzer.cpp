#include "PlayerBehaviorAnalyzer.h"
#include "AIWorldDirectorSubsystem.h"
#include "AntiCheatSystemCore/Public/ServerMetricsSubsystem.h"

void UPlayerBehaviorAnalyzer::Initialize(UAIWorldDirectorSubsystem* InDirector)
{
    Director = InDirector;
    if (Director && Director->GetWorld())
    {
        Metrics = Director->GetWorld()->GetSubsystem<UServerMetricsSubsystem>();
    }
}

void UPlayerBehaviorAnalyzer::RecordCombatData(const FUniqueNetIdRepl& PlayerID, float DPS, float TTK, float SurvivalTime, float DamageTaken)
{
    FPlayerBehaviorData& Data = PlayerData.FindOrAdd(PlayerID);
    Data.DPS = DPS;
    Data.TTK = TTK;
    Data.SurvivalTime = SurvivalTime;
    Data.DamageTaken = DamageTaken;
    Data.LastUpdate = FDateTime::UtcNow();
}

void UPlayerBehaviorAnalyzer::RecordAbilityUse(const FUniqueNetIdRepl& PlayerID)
{
    FPlayerBehaviorData& Data = PlayerData.FindOrAdd(PlayerID);
    Data.AbilityUses++;
}

void UPlayerBehaviorAnalyzer::RecordDodge(const FUniqueNetIdRepl& PlayerID)
{
    FPlayerBehaviorData& Data = PlayerData.FindOrAdd(PlayerID);
    Data.DodgeCount++;
}

void UPlayerBehaviorAnalyzer::RecordBlock(const FUniqueNetIdRepl& PlayerID)
{
    FPlayerBehaviorData& Data = PlayerData.FindOrAdd(PlayerID);
    Data.BlockCount++;
}

void UPlayerBehaviorAnalyzer::UpdateAnalysis()
{
    CleanupOldData();
    EnforceMaxTrackedPlayers();
    UpdateAggregates();
}

void UPlayerBehaviorAnalyzer::UpdateAggregates()
{
    TotalDPS = 0.f;
    TotalTTK = 0.f;
    PlayerCount = 0;
    RangedCount = 0;
    MeleeCount = 0;
    TotalDodges = 0;
    TotalBlocks = 0;
    TotalAbilities = 0;

    for (const auto& Pair : PlayerData)
    {
        const FPlayerBehaviorData& Data = Pair.Value;
        TotalDPS += Data.DPS;
        TotalTTK += Data.TTK;
        PlayerCount++;
        // Упрощённое определение роли (можно уточнить позже)
        TotalDodges += Data.DodgeCount;
        TotalBlocks += Data.BlockCount;
        TotalAbilities += Data.AbilityUses;
    }

    if (PlayerCount > 0)
    {
        TotalDPS /= PlayerCount;
        TotalTTK /= PlayerCount;
    }
}

float UPlayerBehaviorAnalyzer::GetAverageDPS() const
{
    return PlayerCount > 0 ? TotalDPS : 0.f;
}

float UPlayerBehaviorAnalyzer::GetAverageTTK() const
{
    return PlayerCount > 0 ? TotalTTK : 0.f;
}

float UPlayerBehaviorAnalyzer::GetRangedUsage() const
{
    return PlayerCount > 0 ? (float)RangedCount / PlayerCount : 0.f;
}

float UPlayerBehaviorAnalyzer::GetMeleeUsage() const
{
    return PlayerCount > 0 ? (float)MeleeCount / PlayerCount : 0.f;
}

void UPlayerBehaviorAnalyzer::CleanupOldData()
{
    FDateTime Now = FDateTime::UtcNow();
    TArray<FUniqueNetIdRepl> ToRemove;
    for (const auto& Pair : PlayerData)
    {
        if ((Now - Pair.Value.LastUpdate).GetTotalMinutes() > DATA_EXPIRATION_MINUTES)
        {
            ToRemove.Add(Pair.Key);
        }
    }
    for (const auto& ID : ToRemove)
    {
        PlayerData.Remove(ID);
    }
}

void UPlayerBehaviorAnalyzer::EnforceMaxTrackedPlayers()
{
    if (PlayerData.Num() <= MAX_TRACKED_PLAYERS) return;

    TArray<TPair<FUniqueNetIdRepl, FDateTime>> PlayersWithTime;
    for (const auto& Pair : PlayerData)
    {
        PlayersWithTime.Emplace(Pair.Key, Pair.Value.LastUpdate);
    }
    PlayersWithTime.Sort([](const auto& A, const auto& B) { return A.Value < B.Value; });

    int32 Excess = PlayerData.Num() - MAX_TRACKED_PLAYERS;
    for (int32 i = 0; i < Excess; ++i)
    {
        PlayerData.Remove(PlayersWithTime[i].Key);
    }
}
