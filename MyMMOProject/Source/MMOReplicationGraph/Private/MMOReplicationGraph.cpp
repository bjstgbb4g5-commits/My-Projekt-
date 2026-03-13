#include "MMOReplicationGraph.h"
#include "SpatialGridNode.h"
#include "ReplicationGraphNodes.h"

UMMOReplicationGraph::UMMOReplicationGraph()
{
}

void UMMOReplicationGraph::InitGlobalActorClassSettings()
{
    Super::InitGlobalActorClassSettings();
    // Настройки классов акторов
}

void UMMOReplicationGraph::InitGlobalGraphNodes()
{
    Super::InitGlobalGraphNodes();

    AlwaysRelevantNode = CreateNode<UReplicationGraphNode_ActorList>();
    AddGlobalGraphNode(AlwaysRelevantNode);

    SpatialGridNode = CreateNode<USpatialGridNode>();
    SpatialGridNode->SetGridSize(5000.f);
    SpatialGridNode->SetWorldBounds(FBox(FVector(-100000, -100000, -50000), FVector(100000, 100000, 50000)));
    AddGlobalGraphNode(SpatialGridNode);
}

void UMMOReplicationGraph::RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo, FGlobalReplicationGraphParams& Params)
{
    if (ActorInfo.Actor->bAlwaysRelevant)
    {
        AlwaysRelevantNode->NotifyAddNetworkActor(ActorInfo);
    }
    else
    {
        SpatialGridNode->NotifyAddNetworkActor(ActorInfo);
    }
}

void UMMOReplicationGraph::RouteRemoveNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo)
{
    SpatialGridNode->NotifyRemoveNetworkActor(ActorInfo);
    AlwaysRelevantNode->NotifyRemoveNetworkActor(ActorInfo);
}
