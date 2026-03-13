#pragma once

#include "CoreMinimal.h"
#include "BossCharacter.h"
#include "WorldBossCharacter.generated.h"

UCLASS()
class WORLDBOSSSYSTEMCORE_API AWorldBossCharacter : public ABossCharacter
{
    GENERATED_BODY()

public:
    AWorldBossCharacter();

    UPROPERTY(BlueprintReadOnly, Category = "WorldBoss")
    int32 BossID = -1;

    UFUNCTION(BlueprintCallable, Category = "WorldBoss")
    void InitializeHealth(float Health);

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
    virtual void BeginPlay() override;
};
