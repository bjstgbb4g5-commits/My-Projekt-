#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "ThreatComponent.h"
#include "AIEnemyController.generated.h"

UCLASS()
class AICORESYSTEM_API AAIEnemyController : public AAIController
{
    GENERATED_BODY()

public:
    AAIEnemyController();

    UFUNCTION(BlueprintCallable, Category = "AI")
    UThreatComponent* GetThreatComponent() const { return ThreatComponent; }

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAIPerceptionComponent* PerceptionComp;

    UPROPERTY()
    UThreatComponent* ThreatComponent;

    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
