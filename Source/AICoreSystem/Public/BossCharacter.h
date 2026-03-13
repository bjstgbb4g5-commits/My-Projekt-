#pragma once

#include "CoreMinimal.h"
#include "AIEnemyCharacter.h"
#include "BossPhaseComponent.h"
#include "BossCharacter.generated.h"

UCLASS()
class AICORESYSTEM_API ABossCharacter : public AAIEnemyCharacter
{
    GENERATED_BODY()

public:
    ABossCharacter();

    UFUNCTION(BlueprintCallable, Category = "Boss")
    void UpdateBossPhase(float HealthPercent);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss")
    UBossPhaseComponent* PhaseComponent;
};
