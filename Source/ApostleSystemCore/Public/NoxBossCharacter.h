#pragma once

#include "CoreMinimal.h"
#include "BossCharacter.h"
#include "NoxBossCharacter.generated.h"

UCLASS()
class PVEWORLDCORE_API ANoxBossCharacter : public ABossCharacter
{
    GENERATED_BODY()

public:
    ANoxBossCharacter();

    UFUNCTION(BlueprintCallable, Category = "Nox")
    void InitializeHealth(float Health);

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
    virtual void BeginPlay() override;

    // Фазы босса
    void UpdatePhase(float HealthPercent);
    void Phase1(); // обычные атаки
    void Phase2(); // новые механики
    void Phase3(); // берсерк

    UPROPERTY()
    class UNoxEventSubsystem* EventSubsystem;
};
