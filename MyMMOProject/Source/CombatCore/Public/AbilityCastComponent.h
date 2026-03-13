#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityInstance.h"
#include "AbilityCastComponent.generated.h"

class UCharacterEntity;
class UGameConfig;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCastStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCastCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCastInterrupted);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMBATCORE_API UAbilityCastComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAbilityCastComponent();

    void Initialize(UCharacterEntity* InCharacter, UGameConfig* InConfig);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool CanCast(const FAbilityInstance& Ability, AActor* Target) const;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StartCast(const FAbilityInstance& Ability, AActor* Target, int32 SlotIndex);

    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Combat")
    void Server_RequestCast(int32 AbilitySlot, AActor* Target);

    // События
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCastStarted OnCastStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCastCompleted OnCastCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCastInterrupted OnCastInterrupted;

protected:
    UPROPERTY()
    UCharacterEntity* OwnerEntity;

    UPROPERTY()
    UGameConfig* Config;

    bool bIsCasting;
    bool bIsGCDActive;
    int32 CurrentCastingSlot;
    TWeakObjectPtr<AActor> CurrentTarget;

    FTimerHandle CastTimerHandle;
    FTimerHandle GCDTimerHandle;

    UFUNCTION()
    void OnOwnerDamaged(float DamagePercent);

    void StartGCD();
    void InterruptCast(const FString& Reason);
    void CompleteCast();

    virtual void BeginPlay() override;
};
