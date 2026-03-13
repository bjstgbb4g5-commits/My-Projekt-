#pragma once

#include "CoreMinimal.h"
#include "ReplicationGraph.h"
#include "SpatialGridNode.generated.h"

UCLASS()
class MMOREPLICATIONGRAPH_API USpatialGridNode : public UReplicationGraphNode
{
    GENERATED_BODY()

public:
    USpatialGridNode();
    virtual void NotifyAddNetworkActor(const FNewReplicatedActorInfo& Actor) override;
    virtual bool NotifyRemoveNetworkActor(const FNewReplicatedActorInfo& ActorInfo, bool bWarnIfNotFound = true) override;
    virtual void GatherActorListsForConnection(const FConnectionGatherActorListParameters& Params) override;

    void SetGridSize(float InCellSize);
    void SetWorldBounds(const FBox& InWorldBounds);

private:
    float CellSize;
    FBox WorldBounds;

    // Используем хэш-таблицу с плоским индексом
    static constexpr int32 GridHashMultiplier = 10000; // достаточно для мира ±100 км при клетке 10 м
    TMap<int32, TArray<AActor*>> GridCells;

    int32 GetCellHash(const FIntVector& Cell) const;
    FIntVector GetCellIndex(const FVector& Location) const;
};
