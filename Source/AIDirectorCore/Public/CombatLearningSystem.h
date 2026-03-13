#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIDirectorTypes.h"
#include "CombatLearningSystem.generated.h"

class UAIWorldDirectorSubsystem;
class UPlayerBehaviorAnalyzer;

UCLASS()
class AIDIRECTORCORE_API UCombatLearningSystem : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UAIWorldDirectorSubsystem* InDirector);
    void UpdateEfficiency();

    float GetGlobalEfficiency() const { return GlobalEfficiency; }
    float GetPlayerEfficiency(const FUniqueNetIdRepl& PlayerID) const;

private:
    UPROPERTY()
    UAIWorldDirectorSubsystem* Director;

    UPROPERTY()
    UPlayerBehaviorAnalyzer* Analyzer;

    float GlobalEfficiency = 1.0f;
    static constexpr float SMOOTHING_FACTOR = 0.2f;

    TMap<FUniqueNetIdRepl, FPlayerEfficiency> PlayerEfficiency;

    void UpdateGlobalEfficiency(float MeasuredEfficiency);
};
