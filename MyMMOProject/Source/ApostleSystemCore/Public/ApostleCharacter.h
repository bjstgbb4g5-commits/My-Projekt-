#pragma once

#include "CoreMinimal.h"
#include "BossCharacter.h"
#include "ApostleTypes.h"
#include "ApostleCharacter.generated.h"

UCLASS()
class APOSTLESYSTEMCORE_API AApostleCharacter : public ABossCharacter
{
    GENERATED_BODY()

public:
    AApostleCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apostle")
    EApostleType ApostleType;

    UFUNCTION(BlueprintCallable, Category = "Apostle")
    void InitializeHealth(float Health);

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
    virtual void BeginPlay() override;
};
