#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerformanceTypes.h"
#include "AILODComponent.generated.h"

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class WORLDPERFORMANCECORE_API UAILODComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAILODComponent();

    UFUNCTION(BlueprintCallable, Category = "AI LOD")
    void UpdateLOD(float Distance);

    EAILODLevel GetCurrentLOD() const { return CurrentLOD; }
    void SetInCombat(bool bCombat);

protected:
    virtual void BeginPlay() override;

private:
    EAILODLevel CurrentLOD;
    bool bInCombat;

    float ActiveDistance = 6000.f;
    float ReducedDistance = 15000.f;

    void SetLOD(EAILODLevel NewLOD);
    float GetDistanceToNearestPlayer() const;
};
