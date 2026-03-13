#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameConfig.generated.h"

UCLASS(BlueprintType)
class COMBATCORE_API UGameConfig : public UDataAsset
{
    GENERATED_BODY()

public:
    // PvP модификатор
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PvP")
    float PvPDamageModifier = 0.75f;

    // GCD
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GCD")
    float BaseGCD = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GCD")
    float MinGCD = 0.75f;

    // Ресурсы
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
    float EnergyRegenPerSecondOutOfCombat = 0.05f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
    float EnergyRegenPerSecondInCombat = 0.02f;

    // AoE
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AoE")
    float AoEDamageMultiplier = 0.85f;

    // Прерывание
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interrupt")
    float InterruptDamageThreshold = 0.1f; // 10% от макс. здоровья
};
