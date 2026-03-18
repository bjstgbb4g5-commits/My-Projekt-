#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacter.generated.h"

class UNPCLifeComponent;
class UNPCActivityComponent;
class UNPCPerceptionComponent;
class UUtilityAIComponent;

UCLASS()
class NPCLIFESIMULATIONCORE_API ANPCCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ANPCCharacter();

protected:

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    /*
    Core NPC components
    */

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="NPC")
    UNPCLifeComponent* LifeComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="NPC")
    UNPCActivityComponent* ActivityComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="NPC")
    UNPCPerceptionComponent* PerceptionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="NPC")
    UUtilityAIComponent* UtilityAIComponent;
};
