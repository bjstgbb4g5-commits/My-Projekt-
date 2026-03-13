#include "ResourceSimulationSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "WorldGeneratorCore/Public/WorldGeneratorSubsystem.h"

UResourceSimulationSubsystem::UResourceSimulationSubsystem()
    : NextNodeID(1)
{
}

void UResourceSimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        ScheduleSimulation(1800.0f); // каждые 30 минут
    }
}

void UResourceSimulationSubsystem::Deinitialize()
{
    if (IsServer())
    {
        GetWorld()->GetTimerManager().ClearTimer(SimulationTimerHandle);
    }
    ResourceNodes.Empty();
    Super::Deinitialize();
}

bool UResourceSimulationSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

void UResourceSimulationSubsystem::ScheduleSimulation(float IntervalSeconds)
{
    GetWorld()->GetTimerManager().SetTimer(SimulationTimerHandle, this, &UResourceSimulationSubsystem::OnSimulationTimer, IntervalSeconds, true);
}

void UResourceSimulationSubsystem::OnSimulationTimer()
{
    SimulateResourceMigration();
}

void UResourceSimulationSubsystem::SimulateResourceMigration()
{
    if (!IsServer()) return;
    if (ResourceNodes.Num() >= MAX_RESOURCE_NODES) return;

    // Удаляем истощённые узлы
    TArray<int32> ToRemove;
    for (const FResourceNode& Node : ResourceNodes)
    {
        if (Node.RemainingAmount <= 0)
            ToRemove.Add(Node.NodeID);
    }
    for (int32 ID : ToRemove)
        RemoveResourceNode(ID);

    // Создаём новые узлы (пример)
    UWorldGeneratorSubsystem* WorldGen = GetWorld()->GetSubsystem<UWorldGeneratorSubsystem>();
    if (!WorldGen) return;

    const auto& Regions = WorldGen->GetRegions();
    for (int32 i = 0; i < Regions.Num(); ++i)
    {
        if (FMath::FRand() < 0.1f) // 10% шанс
        {
            EResourceType Type = EResourceType::Wood;
            switch (Regions[i].Biome)
            {
            case EBiomeType::Forest: Type = EResourceType::Wood; break;
            case EBiomeType::Mountains: Type = EResourceType::Ore; break;
            case EBiomeType::Plains: Type = EResourceType::Fiber; break;
            default: Type = EResourceType::Crystal; break;
            }
            int32 Tier = FMath::RandRange(1, 3);
            CreateResourceNode(i, Type, Tier);
        }
    }
}

void UResourceSimulationSubsystem::CreateResourceNode(int32 RegionID, EResourceType Type, int32 Tier)
{
    FResourceNode Node;
    Node.NodeID = NextNodeID++;
    Node.RegionID = RegionID;
    Node.ResourceType = Type;
    Node.TotalAmount = Tier * 1000;
    Node.RemainingAmount = Node.TotalAmount;
    Node.Tier = Tier;
    Node.SpawnTime = GetWorld()->GetTimeSeconds();
    ResourceNodes.Add(Node);
}

void UResourceSimulationSubsystem::RemoveResourceNode(int32 NodeID)
{
    ResourceNodes.RemoveAll([NodeID](const FResourceNode& N) { return N.NodeID == NodeID; });
}
