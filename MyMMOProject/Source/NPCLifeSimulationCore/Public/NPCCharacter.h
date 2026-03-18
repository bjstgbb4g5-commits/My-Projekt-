#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacter.generated.h"

class UNPCLifeComponent;

UCLASS()
class NPCLIFESIMULATIONCORE_API ANPCCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ANPCCharacter();

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="NPC")
    UNPCLifeComponent* LifeComponent;
};
