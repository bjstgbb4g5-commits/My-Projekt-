#include "AntiCheatSubsystem.h"
#include "ServerMetricsSubsystem.h"
#include "BanManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "Misc/DateTime.h"

UAntiCheatSubsystem::UAntiCheatSubsystem()
{
}

void UAntiCheatSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        Metrics = GetWorld()->GetSubsystem<UServerMetricsSubsystem>();
        BanManager = GetWorld()->GetSubsystem<UBanManager>();
        if (!BanManager)
        {
            UE_LOG(LogTemp, Error, TEXT("UAntiCheatSubsystem: BanManager not found!"));
        }
    }
}

void UAntiCheatSubsystem::Deinitialize()
{
    SuspicionData.Empty();
    AbilitySpamTracker.Empty();
    EconomySpikeTracker.Empty();
    Super::Deinitialize();
}

bool UAntiCheatSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UAntiCheatSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UAntiCheatSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UAntiCheatSubsystem, STATGROUP_Tickables);
}

void UAntiCheatSubsystem::Tick(float DeltaTime)
{
    FDateTime Now = FDateTime::UtcNow();

    // Очистка старых подозрений
    TArray<FUniqueNetIdRepl> ToRemove;
    for (auto& Pair : SuspicionData)
    {
        if ((Now - Pair.Value.LastUpdate).GetTotalMinutes() > 30)
            ToRemove.Add(Pair.Key);
    }
    for (const auto& ID : ToRemove)
        SuspicionData.Remove(ID);

    // Очистка старых экономических пиков
    TArray<FUniqueNetIdRepl> ToRemoveEconomy;
    for (auto& Pair : EconomySpikeTracker)
    {
        if ((Now - Pair.Value.StartTime).GetTotalSeconds() > ECONOMY_SPIKE_WINDOW)
            ToRemoveEconomy.Add(Pair.Key);
    }
    for (const auto& ID : ToRemoveEconomy)
        EconomySpikeTracker.Remove(ID);

    // Decay подозрений
    DecayAccumulator += DeltaTime;
    if (DecayAccumulator >= DECAY_INTERVAL)
    {
        DecayAccumulator -= DECAY_INTERVAL;
        for (auto& Pair : SuspicionData)
        {
            Pair.Value.SuspicionScore = FMath::Max(0, Pair.Value.SuspicionScore - DECAY_AMOUNT);
        }
    }
}

void UAntiCheatSubsystem::ReportMovement(const FUniqueNetIdRepl& PlayerID, const FVector& Location, float Speed, float DeltaTime)
{
    if (!IsServer() || !PlayerID.IsValid()) return;

    if (Speed > MAX_ALLOWED_SPEED)
    {
        AddSuspicion(PlayerID, ECheatType::SpeedHack, 40);
    }

    FPlayerSuspicionData& Data = SuspicionData.FindOrAdd(PlayerID);
    Data.PlayerID = PlayerID;

    if (!Data.LastLocation.IsZero() && DeltaTime > 0.f)
    {
        float Distance = FVector::Dist(Location, Data.LastLocation);
        float MaxAllowed = MAX_ALLOWED_SPEED * DeltaTime * 3.f;
        if (Distance > MaxAllowed)
        {
            AddSuspicion(PlayerID, ECheatType::TeleportHack, 50);
        }
    }

    Data.LastLocation = Location;
    Data.LastRecordedSpeed = Speed;
    Data.LastUpdate = FDateTime::UtcNow();
}

void UAntiCheatSubsystem::ReportDamage(const FUniqueNetIdRepl& PlayerID, float Damage, float MaxAbilityDamage)
{
    if (!IsServer() || !PlayerID.IsValid()) return;
    if (MaxAbilityDamage <= 0) return;

    float AllowedMax = MaxAbilityDamage * DAMAGE_HACK_MULTIPLIER;
    if (Damage > AllowedMax)
    {
        AddSuspicion(PlayerID, ECheatType::DamageHack, 40);
    }

    if (Metrics) Metrics->RecordDamage(PlayerID, static_cast<int32>(Damage));
}

void UAntiCheatSubsystem::ReportGoldGain(const FUniqueNetIdRepl& PlayerID, int32 Amount)
{
    if (!IsServer() || !PlayerID.IsValid()) return;

    FDateTime Now = FDateTime::UtcNow();

    FEconomySpikeData& Spike = EconomySpikeTracker.FindOrAdd(PlayerID);
    if (!Spike.StartTime.IsValid())
    {
        Spike.StartTime = Now;
        Spike.TotalGain = 0;
    }
    Spike.TotalGain += Amount;

    if ((Now - Spike.StartTime).GetTotalSeconds() > ECONOMY_SPIKE_WINDOW)
    {
        Spike.StartTime = Now;
        Spike.TotalGain = Amount;
    }

    if (Spike.TotalGain > ECONOMY_SPIKE_LIMIT)
    {
        AddSuspicion(PlayerID, ECheatType::EconomyHack, 50);
    }

    if (Metrics) Metrics->RecordGold(PlayerID, Amount);
}

void UAntiCheatSubsystem::ReportAbilityUse(const FUniqueNetIdRepl& PlayerID)
{
    if (!IsServer() || !PlayerID.IsValid()) return;

    FDateTime Now = FDateTime::UtcNow();

    FRecentAbilityData& Tracker = AbilitySpamTracker.FindOrAdd(PlayerID);
    Tracker.RecentTimestamps.Add(Now);
    if (Tracker.RecentTimestamps.Num() > 20)
        Tracker.RecentTimestamps.RemoveAt(0);

    Tracker.RecentTimestamps.RemoveAll([Now](const FDateTime& T) { return (Now - T).GetTotalMilliseconds() > 1000; });

    if (Tracker.RecentTimestamps.Num() >= ABILITY_SPAM_THRESHOLD)
    {
        bool bSpam = true;
        for (int32 i = 0; i < Tracker.RecentTimestamps.Num() - 1; ++i)
        {
            double DiffMs = (Tracker.RecentTimestamps[i+1] - Tracker.RecentTimestamps[i]).GetTotalMilliseconds();
            if (DiffMs > MIN_ABILITY_INTERVAL_MS)
            {
                bSpam = false;
                break;
            }
        }
        if (bSpam)
        {
            AddSuspicion(PlayerID, ECheatType::AbilitySpam, 30);
        }
    }

    if (Metrics) Metrics->RecordAction(PlayerID);
}

void UAntiCheatSubsystem::ReportSuspiciousAction(const FUniqueNetIdRepl& PlayerID, ECheatType Type, int32 Score)
{
    AddSuspicion(PlayerID, Type, Score);
}

void UAntiCheatSubsystem::OnPlayerLogout(const FUniqueNetIdRepl& PlayerID)
{
    SuspicionData.Remove(PlayerID);
    AbilitySpamTracker.Remove(PlayerID);
    EconomySpikeTracker.Remove(PlayerID);
}

void UAntiCheatSubsystem::AddSuspicion(const FUniqueNetIdRepl& PlayerID, ECheatType Type, int32 Score)
{
    FPlayerSuspicionData& Data = SuspicionData.FindOrAdd(PlayerID);
    Data.PlayerID = PlayerID;
    Data.SuspicionScore += Score;
    Data.LastUpdate = FDateTime::UtcNow();

    switch (Type)
    {
    case ECheatType::SpeedHack:   Data.SpeedFlags++; break;
    case ECheatType::DamageHack:  Data.DamageFlags++; break;
    case ECheatType::TeleportHack: Data.TeleportFlags++; break;
    case ECheatType::EconomyHack: Data.EconomyFlags++; break;
    case ECheatType::Botting:     Data.BotFlags++; break;
    case ECheatType::PacketInjection: Data.PacketFlags++; break;
    case ECheatType::AbilitySpam: Data.AbilitySpamFlags++; break;
    default: break;
    }

    EvaluatePlayer(PlayerID);
}

void UAntiCheatSubsystem::EvaluatePlayer(const FUniqueNetIdRepl& PlayerID)
{
    FPlayerSuspicionData* Data = SuspicionData.Find(PlayerID);
    if (!Data) return;

    if (Data->SuspicionScore >= AUTOBAN_SCORE)
    {
        ApplyPenalty(PlayerID, TEXT("Autoban: suspicion score exceeded"));
    }
    else if (Data->SuspicionScore >= AUTOKICK_SCORE)
    {
        if (BanManager)
        {
            BanManager->KickPlayer(PlayerID, TEXT("Suspicion score too high"));
        }
    }
}

void UAntiCheatSubsystem::ApplyPenalty(const FUniqueNetIdRepl& PlayerID, const FString& Reason)
{
    UE_LOG(LogTemp, Warning, TEXT("Player %s banned: %s"), *PlayerID.ToString(), *Reason);
    if (BanManager)
    {
        BanManager->BanPlayer(PlayerID, Reason);
    }
}
