#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterStats.h"
#include "AbilityDataAsset.generated.h"

UENUM(BlueprintType)
enum class EAoECenterType : uint8
{
    Caster,
    Target,
    Ground
};

UCLASS(BlueprintType)
class COMBATCORE_API UAbilityDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
    FString AbilityName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    EElementType Element = EElementType::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    EResourceType RequiredResource = EResourceType::Mana;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float ResourceCost = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float BaseSkillPower = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float CastTime = 0.f; // 0 = мгновенно

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    bool bIsInterruptible = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    bool bIsChanneled = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float Cooldown = 3.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float Range = 10.f;

    // AoE параметры
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AoE")
    bool bIsAoE = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AoE")
    float AoERadius = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AoE")
    EAoECenterType AoECenter = EAoECenterType::Target;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AoE")
    TArray<TEnumAsByte<ECollisionChannel>> TargetChannels;

    // Эффекты контроля
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control")
    bool bHasControlEffect = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control")
    float ControlDuration = 0.f;
};
