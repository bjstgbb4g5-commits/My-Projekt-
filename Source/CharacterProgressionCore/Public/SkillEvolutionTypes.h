#pragma once

#include "CoreMinimal.h"
#include "SkillEvolutionTypes.generated.h"

USTRUCT(BlueprintType)
struct FWeaponSkill
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 SkillID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RequiredMasteryLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 BranchID = 0; // 1=Offensive, 2=Control, 3=Utility

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float BaseDamage = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Cooldown = 0.f;
};

USTRUCT(BlueprintType)
struct FSkillEvolution
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 SkillID = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 Level = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float DamageModifier = 1.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bUnlockedModifier = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 ChosenModifierID = 0;
};
