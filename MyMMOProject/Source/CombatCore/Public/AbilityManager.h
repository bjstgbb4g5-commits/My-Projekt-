#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityInstance.h"
#include "AbilityManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMBATCORE_API UAbilityManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UAbilityManager();

    UFUNCTION(BlueprintCallable, Category = "Ability")
    void AddAbility(const FAbilityInstance& Ability);

    UFUNCTION(BlueprintCallable, Category = "Ability")
    FAbilityInstance GetAbility(int32 SlotIndex) const;

    UFUNCTION(BlueprintCallable, Category = "Ability")
    bool IsOnCooldown(int32 SlotIndex) const;

    UFUNCTION(BlueprintCallable, Category = "Ability")
    void PutOnCooldown(int32 SlotIndex);

    // Tick для проверки кулдаунов
    void TickCooldowns(float DeltaTime);

protected:
    UPROPERTY()
    TArray<FAbilityInstance> Abilities;

    UPROPERTY()
    TMap<int32, float> CooldownTimers;
};
