#include "WorldEcosystemSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "AIEnemyCharacter.h"

UWorldEcosystemSubsystem::UWorldEcosystemSubsystem()
{
}

void UWorldEcosystemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        ScheduleUpdate(ECOSYSTEM_UPDATE_INTERVAL);
        // Загрузить зоны из WorldGenerator или конфига
    }
}

void UWorldEcosystemSubsystem::Deinitialize()
{
    if (IsServer())
    {
        GetWorld()->GetTimerManager().ClearTimer(UpdateTimerHandle);
    }
    Super::Deinitialize();
}

bool UWorldEcosystemSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

void UWorldEcosystemSubsystem::ScheduleUpdate(float IntervalSeconds)
{
    GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, this, &UWorldEcosystemSubsystem::OnUpdateTimer, IntervalSeconds, true);
}

void UWorldEcosystemSubsystem::OnUpdateTimer()
{
    UpdateEcosystem();
}

void UWorldEcosystemSubsystem::UpdateEcosystem()
{
    for (FEcosystemZone& Zone : Zones)
    {
        SpawnZonePopulation(Zone.ZoneID);
    }
    DespawnFarCreatures();
}

void UWorldEcosystemSubsystem::SpawnZonePopulation(int32 ZoneID)
{
    // Заглушка: будет спавнить животных/мобов в зависимости от зоны
}

void UWorldEcosystemSubsystem::DespawnFarCreatures()
{
    // Заглушка: удалять мобов далеко от игроков
}
