#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetValidator.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMBATCORE_API UTargetValidator : public UActorComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool IsTargetValid(AActor* Caster, AActor* Target, float Range) const;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool IsTargetValidWithAbility(AActor* Caster, AActor* Target, UAbilityDataAsset* Ability) const;
};
