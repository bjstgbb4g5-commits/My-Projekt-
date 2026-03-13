#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AbilityInstance.h"
#include "CharacterEntity.h"
#include "GameConfig.h"
#include "ElementMatrix.h"
#include "CombatManager.generated.h"

USTRUCT()
struct FFocusAttack
{
    GENERATED_BODY()

    AActor* Attacker;
    float LastHitTime;
};

UCLASS()
class COMBATCORE_API UCombatManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UCombatManager();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void ApplyAbility(UCharacterEntity* Caster, AActor* TargetActor, const FAbilityInstance& Ability, int32 AbilitySlot);
    void ApplyDamage(UCharacterEntity* Caster, UCharacterEntity* Target, float Damage);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void RequestAttack(UCharacterEntity* Caster, AActor* Target, int32 SlotIndex);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    UGameConfig* Config;

    UPROPERTY(EditDefaultsOnly, Category = "Config")
    UElementMatrix* ElementMatrix;

private:
    TMap<AActor*, TArray<FFocusAttack>> FocusFireTracker;
    void CleanupFocusFire(float CurrentTime);
    float CalculateFocusFireBonus(AActor* Target, float CurrentTime, AActor* CurrentAttacker);
};
