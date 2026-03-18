
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UtilityAITypes.h"
#include "UtilityAIAction.generated.h"

class ANPCCharacter;

UCLASS(Abstract, Blueprintable, EditInlineNew)
class UTILITYAICORE_API UUtilityAIAction : public UObject
{
    GENERATED_BODY()

public:
    UUtilityAIAction();

    UFUNCTION(BlueprintNativeEvent, Category = "Utility AI")
    float CalculateScore(ANPCCharacter* NPC) const;
    virtual float CalculateScore_Implementation(ANPCCharacter* NPC) const;

    UFUNCTION(BlueprintNativeEvent, Category = "Utility AI")
    void Execute(ANPCCharacter* NPC);
    virtual void Execute_Implementation(ANPCCharacter* NPC);

    ENPCUtilityActionType GetActionType() const { return ActionType; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
    ENPCUtilityActionType ActionType = ENPCUtilityActionType::Idle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
    float BaseUtility = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
    float CooldownTime = 5.0f;

    float LastExecuteTime = 0.0f;
};
