#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "AIOptimization.generated.h"

USTRUCT(BlueprintType)
struct FAIOptimizationSettings : public FTableRowBase
{
    GENERATED_BODY()

    // Дистанции для уровней оптимизации
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float FullDistance = 3000.f;      // до 30 м
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ReducedDistance = 8000.f;   // до 80 м
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MinimalDistance = 15000.f;  // до 150 м

    // Интервалы тиков для каждого уровня
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float FullTickInterval = 0.1f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ReducedTickInterval = 0.3f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MinimalTickInterval = 1.0f;

    // Отключать логику дальше этого расстояния
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float OffDistance = 20000.f;
};

UCLASS()
class AICORESYSTEM_API UOptimizationManager : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(const FAIOptimizationSettings& Settings);
    EOptimizationTier GetTierForDistance(float Distance) const;

private:
    FAIOptimizationSettings CurrentSettings;
};
