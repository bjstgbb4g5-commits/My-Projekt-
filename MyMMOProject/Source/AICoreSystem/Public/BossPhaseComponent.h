#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AITypes.h"
#include "BossPhaseComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, EBossPhase, NewPhase);

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AICORESYSTEM_API UBossPhaseComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBossPhaseComponent();

    UFUNCTION(BlueprintCallable, Category = "Boss")
    void UpdatePhase(float HealthPercent);

    UFUNCTION(BlueprintCallable, Category = "Boss")
    EBossPhase GetCurrentPhase() const { return CurrentPhase; }

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPhaseChanged OnPhaseChanged;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentPhase)
    EBossPhase CurrentPhase = EBossPhase::Phase1;

    UFUNCTION()
    void OnRep_CurrentPhase();
};
