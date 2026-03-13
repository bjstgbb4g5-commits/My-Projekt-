#include "WorldBossSubsystem.h"
#include "WorldBossCharacter.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

UWorldBossSubsystem::UWorldBossSubsystem()
{
}

void UWorldBossSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    // Восстановление состояния из сохранения (опционально)
}

void UWorldBossSubsystem::Deinitialize()
{
    ActiveBosses.Empty();
    Super::Deinitialize();
}

bool UWorldBossSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UWorldBossSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UWorldBossSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UWorldBossSubsystem, STATGROUP_Tickables);
}

void UWorldBossSubsystem::Tick(float DeltaTime)
{
    // Проверка на респавн мёртвых боссов
    FDateTime Now = FDateTime::UtcNow();
    for (auto& Pair : ActiveBosses)
    {
        if (Pair.Value.State == EWorldBossState::Dead)
        {
            if ((Now - Pair.Value.LastDeathTime).GetTotalSeconds() >= RESPAWN_TIME)
            {
                Pair.Value.State = EWorldBossState::Dormant;
                // Можно заспавнить босса автоматически
            }
        }
    }
}

void UWorldBossSubsystem::SpawnWorldBoss(int32 BossID, FVector Location)
{
    if (!IsServer()) return;

    FWorldBossData Data;
    Data.BossID = BossID;
    Data.SpawnLocation = Location;
    Data.MaxHealth = 500000.f; // База, будет масштабироваться
    Data.CurrentHealth = Data.MaxHealth;
    Data.State = EWorldBossState::Spawned;
    ActiveBosses.Add(BossID, Data);

    // Здесь можно создать актор босса в мире
    UWorld* World = GetWorld();
    if (World)
    {
        FActorSpawnParameters SpawnParams;
        AWorldBossCharacter* Boss = World->SpawnActor<AWorldBossCharacter>(AWorldBossCharacter::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);
        if (Boss)
        {
            Boss->BossID = BossID;
            Boss->InitializeHealth(Data.CurrentHealth);
        }
    }
}

void UWorldBossSubsystem::KillWorldBoss(int32 BossID)
{
    if (!IsServer()) return;

    FWorldBossData* Data = ActiveBosses.Find(BossID);
    if (!Data) return;

    Data->State = EWorldBossState::Dead;
    Data->LastDeathTime = FDateTime::UtcNow();
}

float UWorldBossSubsystem::GetScaledHealth(int32 BossID, int32 PlayersNearby) const
{
    const FWorldBossData* Data = ActiveBosses.Find(BossID);
    if (!Data) return 0.f;

    // Масштабирование: база + корень от числа игроков * коэффициент
    float Scale = 1.f + FMath::Sqrt((float)PlayersNearby) * 0.35f;
    return Data->MaxHealth * Scale;
}

bool UWorldBossSubsystem::IsBossAlive(int32 BossID) const
{
    const FWorldBossData* Data = ActiveBosses.Find(BossID);
    return Data && Data->State == EWorldBossState::Spawned;
}
