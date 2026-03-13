#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatAITypes.h"
#include "CombatStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatStateChanged, ECombatState, NewState);

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AICORESYSTEM_API UCombatStateComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCombatStateComponent();

    UFUNCTION(BlueprintCallable, Category = "Combat State")
    void SetCombatState(ECombatState NewState);

    UFUNCTION(BlueprintCallable, Category = "Combat State")
    ECombatState GetCombatState() const { return CurrentState; }

    UFUNCTION(BlueprintCallable, Category = "Combat State")
    void UpdateStateBasedOnDistance(float DistanceToTarget, float EngageDistance, float FightDistance);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCombatStateChanged OnCombatStateChanged;

    // Оптимизация: частота обновления в зависимости от состояния
    float GetTickInterval() const;

protected:
    UPROPERTY()
    ECombatState CurrentState;

    UPROPERTY()
    float LastStateChangeTime;
};
