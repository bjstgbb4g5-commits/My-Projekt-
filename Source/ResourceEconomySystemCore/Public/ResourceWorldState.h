#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceTypes.h"
#include "ResourceWorldState.generated.h"

UCLASS()
class RESOURCEECONOMYSYSTEMCORE_API AResourceWorldState : public AActor
{
    GENERATED_BODY()

public:
    AResourceWorldState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(ReplicatedUsing = OnRep_ResourceNodes)
    TMap<int32, FResourceNode> ResourceNodes;

    UFUNCTION()
    void OnRep_ResourceNodes();
};
