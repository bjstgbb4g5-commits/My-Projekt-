#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PredatorComponent.generated.h"

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class WORLDECOSYSTEMCORE_API UPredatorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPredatorComponent();

    UFUNCTION()
    void SearchPrey();

    UFUNCTION()
    void AttackPrey(AActor* Target);

protected:
    UPROPERTY()
    float HuntRadius = 1200.f;
};
