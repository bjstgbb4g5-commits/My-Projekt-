#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DifficultyGovernor.generated.h"

UCLASS()
class AIDIRECTORCORE_API UDifficultyGovernor : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UObject* WorldContext);

    float GetMaxHealthMultiplier() const { return MaxHealthMultiplier; }
    float GetMaxDamageMultiplier() const { return MaxDamageMultiplier; }
    float GetMaxAggressionMultiplier() const { return MaxAggressionMultiplier; }
    float GetMinHealthMultiplier() const { return MinHealthMultiplier; }
    float GetMinDamageMultiplier() const { return MinDamageMultiplier; }
    float GetMinAggressionMultiplier() const { return MinAggressionMultiplier; }

private:
    float MaxHealthMultiplier = 1.25f;
    float MaxDamageMultiplier = 1.15f;
    float MaxAggressionMultiplier = 1.2f;
    float MinHealthMultiplier = 0.75f;
    float MinDamageMultiplier = 0.85f;
    float MinAggressionMultiplier = 0.8f;
};
