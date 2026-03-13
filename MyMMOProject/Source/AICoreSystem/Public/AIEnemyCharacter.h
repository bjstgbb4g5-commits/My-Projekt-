#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AITypes.h"
#include "ThreatComponent.h"
#include "AIEnemyCharacter.generated.h"

UCLASS()
class AICORESYSTEM_API AAIEnemyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAIEnemyCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    EAIArchetype Archetype;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UThreatComponent* ThreatComponent;

protected:
    virtual void BeginPlay() override;
};
