#include "WorldPopulationSubsystem.h"
#include "WildlifeSpawner.h"
#include "BanditCampSpawner.h"
#include "VillageSpawner.h"
#include "WorldBossSpawner.h"
#include "NPCPatrolSpawner.h"
#include "Engine/World.h"

UWorldPopulationSubsystem::UWorldPopulationSubsystem()
{
}

void UWorldPopulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    WorldGen = GetWorld()->GetSubsystem<UWorldGeneratorSubsystem>();

    // Создаём дочерние объекты
    WildlifeSpawner = NewObject<UWildlifeSpawner>(this);
    BanditCampSpawner = NewObject<UBanditCampSpawner>(this);
    VillageSpawner = NewObject<UVillageSpawner>(this);
    WorldBossSpawner = NewObject<UWorldBossSpawner>(this);
    NPCPatrolSpawner = NewObject<UNPCPatrolSpawner>(this);
}

void UWorldPopulationSubsystem::Deinitialize()
{
    BanditCamps.Empty();
    WorldBosses.Empty();
    Villages.Empty();
    NPCPatrols.Empty();
    Super::Deinitialize();
}

void UWorldPopulationSubsystem::PopulateWorld()
{
    if (!GetWorld()->IsServer()) return;

    SpawnWildlife();
    SpawnBanditCamps();
    SpawnVillages();
    SpawnWorldBosses();
    SpawnNPCPatrols();

    UE_LOG(LogTemp, Log, TEXT("World populated: %d bandit camps, %d world bosses, %d villages"),
        BanditCamps.Num(), WorldBosses.Num(), Villages.Num());
}

void UWorldPopulationSubsystem::SpawnWildlife()
{
    if (!WildlifeSpawner) return;
    WildlifeSpawner->SpawnWildlife(WorldGen);
}

void UWorldPopulationSubsystem::SpawnBanditCamps()
{
    if (!BanditCampSpawner) return;
    BanditCamps = BanditCampSpawner->GenerateBanditCamps(WorldGen);
}

void UWorldPopulationSubsystem::SpawnVillages()
{
    if (!VillageSpawner) return;
    Villages = VillageSpawner->GenerateVillages(WorldGen);
}

void UWorldPopulationSubsystem::SpawnWorldBosses()
{
    if (!WorldBossSpawner) return;
    WorldBosses = WorldBossSpawner->GenerateWorldBosses(WorldGen);
}

void UWorldPopulationSubsystem::SpawnNPCPatrols()
{
    if (!NPCPatrolSpawner) return;
    NPCPatrols = NPCPatrolSpawner->GenerateNPCPatrols(WorldGen);
}
