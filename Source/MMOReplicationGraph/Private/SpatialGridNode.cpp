#include "SpatialGridNode.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

USpatialGridNode::USpatialGridNode()
{
    CellSize = 5000.f;
    WorldBounds = FBox(FVector(-100000, -100000, -50000), FVector(100000, 100000, 50000));
}

void USpatialGridNode::SetGridSize(float InCellSize)
{
    CellSize = InCellSize;
}

void USpatialGridNode::SetWorldBounds(const FBox& InWorldBounds)
{
    WorldBounds = InWorldBounds;
}

FIntVector USpatialGridNode::GetCellIndex(const FVector& Location) const
{
    int32 X = FMath::FloorToInt(Location.X / CellSize);
    int32 Y = FMath::FloorToInt(Location.Y / CellSize);
    int32 Z = FMath::FloorToInt(Location.Z / CellSize);
    return FIntVector(X, Y, Z);
}

int32 USpatialGridNode::GetCellHash(const FIntVector& Cell) const
{
    // Простая линейная хэш-функция
    return Cell.X + Cell.Y * GridHashMultiplier + Cell.Z * GridHashMultiplier * GridHashMultiplier;
}

void USpatialGridNode::NotifyAddNetworkActor(const FNewReplicatedActorInfo& Actor)
{
    if (!Actor.Actor) return;
    FVector Loc = Actor.Actor->GetActorLocation();
    FIntVector Cell = GetCellIndex(Loc);
    int32 Hash = GetCellHash(Cell);
    GridCells.FindOrAdd(Hash).Add(Actor.Actor);
}

bool USpatialGridNode::NotifyRemoveNetworkActor(const FNewReplicatedActorInfo& ActorInfo, bool bWarnIfNotFound)
{
    if (!ActorInfo.Actor) return false;
    FVector Loc = ActorInfo.Actor->GetActorLocation();
    FIntVector Cell = GetCellIndex(Loc);
    int32 Hash = GetCellHash(Cell);
    TArray<AActor*>* CellActors = GridCells.Find(Hash);
    if (CellActors)
    {
        CellActors->Remove(ActorInfo.Actor);
        if (CellActors->Num() == 0)
            GridCells.Remove(Hash);
        return true;
    }
    return false;
}

void USpatialGridNode::GatherActorListsForConnection(const FConnectionGatherActorListParameters& Params)
{
    FVector ViewLocation = Params.ViewerLocation;
    FIntVector CenterCell = GetCellIndex(ViewLocation);

    // Определяем радиус обзора в клетках (например, 3x3)
    TSet<AActor*> RelevantActors;
    for (int32 dx = -2; dx <= 2; ++dx)
    {
        for (int32 dy = -2; dy <= 2; ++dy)
        {
            FIntVector Cell(CenterCell.X + dx, CenterCell.Y + dy, CenterCell.Z);
            int32 Hash = GetCellHash(Cell);
            TArray<AActor*>* CellActors = GridCells.Find(Hash);
            if (CellActors)
            {
                for (AActor* Actor : *CellActors)
                {
                    RelevantActors.Add(Actor);
                }
            }
        }
    }

    for (AActor* Actor : RelevantActors)
    {
        if (IsValid(Actor))
        {
            Params.OutGatherReplicationLists.AddReplicationActorList({Actor});
        }
    }
}
