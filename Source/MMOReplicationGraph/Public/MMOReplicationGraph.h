#pragma once

#include "CoreMinimal.h"
#include "ReplicationGraph.h"
#include "MMOReplicationGraph.generated.h"

UCLASS()
class MMOREPLICATIONGRAPH_API UMMOReplicationGraph : public UReplicationGraph
{
    GENERATED_BODY()

public:
    UMMOReplicationGraph();

    virtual void InitGlobalActorClassSettings() override;
    virtual void InitGlobalGraphNodes() override;
    virtual void RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo, FGlobalReplicationGraphParams& Params) override;
    virtual void RouteRemoveNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo) override;

private:
    class USpatialGridNode* SpatialGridNode;
    class UReplicationGraphNode_ActorList* AlwaysRelevantNode;
};
