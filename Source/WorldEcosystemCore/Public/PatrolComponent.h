#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolComponent.generated.h"

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class WORLDECOSYSTEMCORE_API UPatrolComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFUNCTION()
    void StartPatrol();

protected:
    UPROPERTY()
    TArray<FVector> PatrolPoints;

    int32 CurrentPoint = 0;
};
