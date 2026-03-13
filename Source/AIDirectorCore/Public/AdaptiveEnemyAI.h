#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIDirectorTypes.h"
#include "AdaptiveEnemyAI.generated.h"

class UAIWorldDirectorSubsystem;
class UDifficultyGovernor;

UCLASS()
class AIDIRECTORCORE_API UAdaptiveEnemyAI : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UAIWorldDirectorSubsystem* InDirector);
    void AdaptMobs();

    FMobAdaptation GetAdaptationForMob(int32 MobID) const;

private:
    UPROPERTY()
    UAIWorldDirectorSubsystem* Director;

    UPROPERTY()
    UDifficultyGovernor* Governor;

    TMap<int32, FMobAdaptation> MobAdaptations;

    static constexpr int32 MAX_ADAPTIVE_MOBS = 200;
    static constexpr float ADAPTATION_LERP_RATE = 0.1f;
    static constexpr float RANDOM_FACTOR_MIN = 0.9f;
    static constexpr float RANDOM_FACTOR_MAX = 1.1f;
};
