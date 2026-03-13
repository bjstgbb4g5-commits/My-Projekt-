#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JusticeEnums.h"
#include "JusticeTypes.h"
#include "JusticeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJusticeStateChanged, EJusticeState, NewState);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUSTICESYSTEM_API UJusticeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UJusticeComponent();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Justice")
    EJusticeState JusticeState;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Justice")
    int32 StrikeCount;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Justice")
    FDateTime LawMarkExpiration;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Justice")
    FDateTime PrisonReleaseTime;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Justice")
    FDateTime ExecutionDebuffEnd;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnJusticeStateChanged OnJusticeStateChanged;

    // Вызывается при совершении серьёзного преступления (убийство игрока, стража)
    UFUNCTION(BlueprintCallable, Category = "Justice")
    void RegisterSeriousCrime();

    // Арест (помещение в тюрьму)
    UFUNCTION(BlueprintCallable, Category = "Justice")
    void Imprison(int32 Minutes);

    // Начать ожидание казни
    UFUNCTION(BlueprintCallable, Category = "Justice")
    void BeginExecutionWaiting();

    // Привести казнь в исполнение
    UFUNCTION(BlueprintCallable, Category = "Justice")
    void ExecutePlayer();

    // Проверить таймеры (вызывается при логине и периодически)
    UFUNCTION(BlueprintCallable, Category = "Justice")
    void CheckJusticeTimers();

    // Получить текущий статус для UI
    UFUNCTION(BlueprintCallable, Category = "Justice")
    float GetMovementSpeedMultiplier() const;

    UFUNCTION(BlueprintCallable, Category = "Justice")
    float GetDamageOutputMultiplier() const;

    UFUNCTION(BlueprintCallable, Category = "Justice")
    float GetDamageTakenMultiplier() const;

    // Должна ли стража атаковать этого персонажа
    UFUNCTION(BlueprintCallable, Category = "Justice")
    bool ShouldGuardsAttack() const;

    // Сброс всех состояний (при смерти)
    UFUNCTION(BlueprintCallable, Category = "Justice")
    void ResetAllControlState();

private:
    bool IsServer() const;
    FDateTime GetUtcNow() const;

    UFUNCTION()
    void OnRep_JusticeState();
};
