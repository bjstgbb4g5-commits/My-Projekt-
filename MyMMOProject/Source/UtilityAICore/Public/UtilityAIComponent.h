
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UtilityAITypes.h"
#include "UtilityAIComponent.generated.h"

class UUtilityAIAction;
class ANPCCharacter;

UCLASS(ClassGroup=(NPC), meta=(BlueprintSpawnableComponent))
class UTILITYAICORE_API UUtilityAIComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UUtilityAIComponent();

    virtual void BeginPlay() override;

    void EvaluateAI();

    UFUNCTION(BlueprintCallable, Category = "Utility AI")
    void AddAction(TSubclassOf<UUtilityAIAction> ActionClass);

    void SetEvaluationInterval(float NewInterval) { EvaluationInterval = NewInterval; }
    ENPCUtilityActionType GetCurrentAction() const { return CurrentAction; }

protected:
    UPROPERTY()
    TArray<UUtilityAIAction*> Actions;

    UPROPERTY()
    ENPCUtilityActionType CurrentAction = ENPCUtilityActionType::Idle;

    UPROPERTY()
    float LastEvaluationTime = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Performance")
    float EvaluationInterval = 0.5f;

private:
    ANPCCharacter* GetNPCCharacter() const;
};
