#include "GuildWarSubsystem.h"
#include "Engine/World.h"
#include "Misc/DateTime.h"

DEFINE_LOG_CATEGORY(LogGuildWarSystem);

UGuildWarSubsystem::UGuildWarSubsystem()
{
}

void UGuildWarSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    // Reserve space to avoid reallocation
    ActiveWars.Reserve(1024);
    FinishedWarHistory.Reserve(4096);
    GuildWarMap.Reserve(1024);
    LastWarDeclaration.Reserve(1024);
}

void UGuildWarSubsystem::Deinitialize()
{
    ActiveWars.Empty();
    FinishedWarHistory.Empty();
    GuildWarMap.Empty();
    LastWarDeclaration.Empty();
    Super::Deinitialize();
}

bool UGuildWarSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UGuildWarSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UGuildWarSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UGuildWarSubsystem, STATGROUP_Tickables);
}

void UGuildWarSubsystem::Tick(float DeltaTime)
{
    TickTimer += DeltaTime;
    if (TickTimer < TICK_INTERVAL)
    {
        return;
    }
    TickTimer = 0.f;

    FDateTime Now = FDateTime::UtcNow();
    TArray<int32> WarsToFinish;

    // Collect wars that have ended
    for (const auto& Pair : ActiveWars)
    {
        const FGuildWar& War = Pair.Value;
        if (War.State == EGuildWarState::Active && Now >= War.EndTime)
        {
            WarsToFinish.Add(Pair.Key);
        }
    }

    // Finish them
    for (int32 WarId : WarsToFinish)
    {
        FinishWar(WarId);
    }

    // Clean up old history
    CleanupOldHistory();
}

int32 UGuildWarSubsystem::DeclareWar(int32 AttackerGuildId, int32 DefenderGuildId)
{
    if (!IsServer()) return -1;

    // Basic validation
    if (AttackerGuildId <= 0 || DefenderGuildId <= 0)
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("DeclareWar: invalid guild ids %d, %d"), AttackerGuildId, DefenderGuildId);
        return -1;
    }
    if (AttackerGuildId == DefenderGuildId)
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("DeclareWar: guild cannot declare war on itself"));
        return -1;
    }
    if (IsGuildAtWar(AttackerGuildId) || IsGuildAtWar(DefenderGuildId))
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("DeclareWar: one or both guilds already at war"));
        return -1;
    }

    // DOS protection
    if (ActiveWars.Num() >= MAX_ACTIVE_WARS)
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("DeclareWar: too many active wars (%d), rejecting"), ActiveWars.Num());
        return -1;
    }

    // Cooldown check
    FDateTime Now = FDateTime::UtcNow();
    if (const FDateTime* Last = LastWarDeclaration.Find(AttackerGuildId))
    {
        FTimespan Age = Now - *Last;
        if (Age.GetTotalHours() < WAR_DECLARATION_COOLDOWN_HOURS)
        {
            UE_LOG(LogGuildWarSystem, Warning, TEXT("DeclareWar: attacker guild %d is on cooldown (%.1f hours remaining)"),
                AttackerGuildId, WAR_DECLARATION_COOLDOWN_HOURS - Age.GetTotalHours());
            return -1;
        }
    }

    // Find a free WarId (with overflow protection)
    int32 StartId = NextWarId;
    while (ActiveWars.Contains(NextWarId) || FinishedWarHistory.Contains(NextWarId))
    {
        NextWarId++;
        if (NextWarId > MAX_WAR_ID)
        {
            NextWarId = 1;
        }
        if (NextWarId == StartId)
        {
            UE_LOG(LogGuildWarSystem, Error, TEXT("DeclareWar: no free WarId available"));
            return -1;
        }
    }

    int32 WarId = NextWarId++;
    FGuildWar War;
    War.WarId = WarId;
    War.AttackerGuildId = AttackerGuildId;
    War.DefenderGuildId = DefenderGuildId;
    War.WarScoreAttacker = 0;
    War.WarScoreDefender = 0;
    War.State = EGuildWarState::Declared;
    // StartTime remains default (not set) until accepted

    ActiveWars.Add(WarId, War);
    LastWarDeclaration.Add(AttackerGuildId, Now);

    UE_LOG(LogGuildWarSystem, Log, TEXT("War %d declared: guild %d vs guild %d"), WarId, AttackerGuildId, DefenderGuildId);
    return WarId;
}

bool UGuildWarSubsystem::AcceptWar(int32 WarId)
{
    if (!IsServer()) return false;

    FGuildWar* War = ActiveWars.Find(WarId);
    if (!War)
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("AcceptWar: war %d not found"), WarId);
        return false;
    }
    if (War->State != EGuildWarState::Declared)
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("AcceptWar: war %d not in Declared state"), WarId);
        return false;
    }
    if (IsGuildAtWar(War->AttackerGuildId) || IsGuildAtWar(War->DefenderGuildId))
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("AcceptWar: one or both guilds already at war"));
        return false;
    }

    FDateTime Now = FDateTime::UtcNow();
    War->State = EGuildWarState::Active;
    War->StartTime = Now;
    War->EndTime = Now + FTimespan::FromHours(DEFAULT_WAR_DURATION_HOURS);

    GuildWarMap.Add(War->AttackerGuildId, WarId);
    GuildWarMap.Add(War->DefenderGuildId, WarId);

    UE_LOG(LogGuildWarSystem, Log, TEXT("War %d accepted and now active until %s"), WarId, *War->EndTime.ToString());
    return true;
}

bool UGuildWarSubsystem::CancelWar(int32 WarId)
{
    if (!IsServer()) return false;

    FGuildWar* War = ActiveWars.Find(WarId);
    if (!War)
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("CancelWar: war %d not found"), WarId);
        return false;
    }
    if (War->State != EGuildWarState::Declared && War->State != EGuildWarState::Active)
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("CancelWar: war %d cannot be cancelled (state %d)"), WarId, (int32)War->State);
        return false;
    }

    War->State = EGuildWarState::Cancelled;

    // Remove from active guild map if present
    GuildWarMap.Remove(War->AttackerGuildId);
    GuildWarMap.Remove(War->DefenderGuildId);

    // Move to history
    FinishedWarHistory.Add(WarId, *War);
    ActiveWars.Remove(WarId);

    UE_LOG(LogGuildWarSystem, Log, TEXT("War %d cancelled"), WarId);
    return true;
}

bool UGuildWarSubsystem::AddWarScore(int32 WarId, int32 GuildId, int32 Score)
{
    if (!IsServer()) return false;

    if (Score < 0)
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("AddWarScore: score %d is negative, ignored"), Score);
        return false;
    }

    FGuildWar* War = ActiveWars.Find(WarId);
    if (!War)
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("AddWarScore: war %d not found"), WarId);
        return false;
    }
    if (War->State != EGuildWarState::Active)
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("AddWarScore: war %d not active"), WarId);
        return false;
    }

    if (GuildId == War->AttackerGuildId)
    {
        War->WarScoreAttacker += Score;
    }
    else if (GuildId == War->DefenderGuildId)
    {
        War->WarScoreDefender += Score;
    }
    else
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("AddWarScore: guild %d not part of war %d"), GuildId, WarId);
        return false;
    }

    // Clamp to prevent overflow
    War->WarScoreAttacker = FMath::Clamp(War->WarScoreAttacker, 0, INT32_MAX);
    War->WarScoreDefender = FMath::Clamp(War->WarScoreDefender, 0, INT32_MAX);

    UE_LOG(LogGuildWarSystem, Verbose, TEXT("War %d: guild %d added score %d, now A:%d D:%d"),
        WarId, GuildId, Score, War->WarScoreAttacker, War->WarScoreDefender);
    return true;
}

bool UGuildWarSubsystem::FinishWar(int32 WarId)
{
    if (!IsServer()) return false;

    FGuildWar* War = ActiveWars.Find(WarId);
    if (!War)
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("FinishWar: war %d not found"), WarId);
        return false;
    }
    if (War->State != EGuildWarState::Active && War->State != EGuildWarState::Declared)
    {
        UE_LOG(LogGuildWarSystem, Warning, TEXT("FinishWar: war %d cannot be finished (state %d)"), WarId, (int32)War->State);
        return false;
    }

    // Determine winner
    int32 WinnerGuildId;
    if (War->WarScoreAttacker > War->WarScoreDefender)
    {
        WinnerGuildId = War->AttackerGuildId;
    }
    else if (War->WarScoreDefender > War->WarScoreAttacker)
    {
        WinnerGuildId = War->DefenderGuildId;
    }
    else
    {
        // Tie: defender wins
        WinnerGuildId = War->DefenderGuildId;
    }

    War->State = EGuildWarState::Finished;

    // Remove from active guild map
    GuildWarMap.Remove(War->AttackerGuildId);
    GuildWarMap.Remove(War->DefenderGuildId);

    // Move to history
    FinishedWarHistory.Add(WarId, *War);
    ActiveWars.Remove(WarId);

    UE_LOG(LogGuildWarSystem, Log, TEXT("War %d finished. Winner: guild %d (A:%d D:%d)"),
        WarId, WinnerGuildId, War->WarScoreAttacker, War->WarScoreDefender);

    // Call territory capture placeholder
    int32 LoserGuildId = (WinnerGuildId == War->AttackerGuildId) ? War->DefenderGuildId : War->AttackerGuildId;
    CaptureTerritoryForGuild(WinnerGuildId, LoserGuildId);

    return true;
}

bool UGuildWarSubsystem::IsGuildAtWar(int32 GuildId) const
{
    return GuildWarMap.Contains(GuildId);
}

int32 UGuildWarSubsystem::GetCurrentWarIdForGuild(int32 GuildId) const
{
    const int32* Found = GuildWarMap.Find(GuildId);
    return Found ? *Found : -1;
}

TArray<FGuildWar> UGuildWarSubsystem::GetGuildWars(int32 GuildId) const
{
    TArray<FGuildWar> Result;
    // Search active wars
    for (const auto& Pair : ActiveWars)
    {
        const FGuildWar& War = Pair.Value;
        if (War.AttackerGuildId == GuildId || War.DefenderGuildId == GuildId)
        {
            Result.Add(War);
        }
    }
    return Result;
}

void UGuildWarSubsystem::CleanupOldHistory()
{
    FDateTime Now = FDateTime::UtcNow();
    TArray<int32> ToRemove;
    for (const auto& Pair : FinishedWarHistory)
    {
        const FGuildWar& War = Pair.Value;
        FTimespan Age = Now - War.EndTime;
        if (Age.GetTotalHours() >= WAR_HISTORY_HOURS)
        {
            ToRemove.Add(Pair.Key);
        }
    }
    for (int32 WarId : ToRemove)
    {
        FinishedWarHistory.Remove(WarId);
    }
    if (ToRemove.Num() > 0)
    {
        UE_LOG(LogGuildWarSystem, Verbose, TEXT("Cleaned up %d old wars from history"), ToRemove.Num());
    }
}

void UGuildWarSubsystem::CaptureTerritoryForGuild(int32 WinnerGuildId, int32 LoserGuildId)
{
    // Placeholder for integration with TerritoryManagerSubsystem
    UE_LOG(LogGuildWarSystem, Log, TEXT("Territory capture placeholder: winner %d, loser %d"), WinnerGuildId, LoserGuildId);
}
