#include "WorldBossSubsystem.h"
#include "Math/UnrealMathUtility.h"

void UWorldBossSubsystem::SpawnWorldBoss(int32 BossID, FVector Location)
{
    FWorldBossData Data;
    Data.BossID = BossID;
    Data.SpawnLocation = Location;
    Data.MaxHealth = 500000.f;
    Data.CurrentHealth = Data.MaxHealth;
    Data.State = EWorldBossState::Spawned;
    ActiveBosses.Add(BossID, Data);
}

void UWorldBossSubsystem::KillWorldBoss(int32 BossID)
{
    FWorldBossData* Data = ActiveBosses.Find(BossID);
    if (!Data) return;
    Data->State = EWorldBossState::Dead;
}

float UWorldBossSubsystem::GetScaledHealth(int32 BossID, int32 PlayersNearby) const
{
    const FWorldBossData* Data = ActiveBosses.Find(BossID);
    if (!Data) return 0.f;
    float Scale = 1.f + FMath::Sqrt((float)PlayersNearby) * 0.35f;
    return Data->MaxHealth * Scale;
}
