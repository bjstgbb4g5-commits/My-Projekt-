#include "RegionStreamingSubsystem.h"
#include "Engine/World.h"
#include "Containers/Ticker.h"

URegionStreamingSubsystem::URegionStreamingSubsystem()
{
}

void URegionStreamingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        WorldGen = GetWorld()->GetSubsystem<UWorldGeneratorSubsystem>();
        TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &URegionStreamingSubsystem::Tick), 1.0f);
    }
}

void URegionStreamingSubsystem::Deinitialize()
{
    if (IsServer())
    {
        FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
        for (auto& Pair : RegionRuntime)
        {
            UnloadRegion(Pair.Key);
        }
        RegionRuntime.Empty();
    }
    Super::Deinitialize();
}

bool URegionStreamingSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool URegionStreamingSubsystem::Tick(float DeltaTime)
{
    if (!IsServer()) return true;

    // Находим игрока (для простоты берём первого)
    UWorld* World = GetWorld();
    if (!World) return true;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!PC || !PC->GetPawn()) return true;

    UpdateRegionStreaming(PC->GetPawn()->GetActorLocation());

    return true;
}

void URegionStreamingSubsystem::UpdateRegionStreaming(FVector PlayerLocation)
{
    if (!WorldGen) return;

    int32 PlayerRegionIdx = WorldGen->GetRegionIndexFromLocation(PlayerLocation);
    if (PlayerRegionIdx < 0) return;

    TSet<int32> RegionsToLoad;
    TArray<int32> Neighbors = WorldGen->GetNeighborRegionIndices(PlayerRegionIdx, 1);
    RegionsToLoad.Add(PlayerRegionIdx);
    for (int32 N : Neighbors)
    {
        RegionsToLoad.Add(N);
    }

    // Выгружаем те, что не в списке
    TArray<int32> ToUnload;
    for (auto& Pair : RegionRuntime)
    {
        if (!RegionsToLoad.Contains(Pair.Key))
        {
            ToUnload.Add(Pair.Key);
        }
    }
    for (int32 Idx : ToUnload)
    {
        UnloadRegion(Idx);
    }

    // Загружаем недостающие
    for (int32 Idx : RegionsToLoad)
    {
        if (!RegionRuntime.Contains(Idx))
        {
            LoadRegion(Idx);
        }
    }
}

void URegionStreamingSubsystem::LoadRegion(int32 RegionIndex)
{
    if (RegionRuntime.Contains(RegionIndex)) return;
    FRegionRuntimeState State;
    State.bIsLoaded = true;
    // Здесь можно спавнить акторы, принадлежащие региону
    RegionRuntime.Add(RegionIndex, State);
}

void URegionStreamingSubsystem::UnloadRegion(int32 RegionIndex)
{
    FRegionRuntimeState* State = RegionRuntime.Find(RegionIndex);
    if (!State) return;
    for (AActor* Actor : State->SpawnedActors)
    {
        if (IsValid(Actor)) Actor->Destroy();
    }
    RegionRuntime.Remove(RegionIndex);
}
