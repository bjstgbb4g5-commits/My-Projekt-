#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIDirectorTypes.h"
#include "AdaptiveBossSystem.generated.h"

class UAIWorldDirectorSubsystem;

UCLASS()
class AIDIRECTORCORE_API UAdaptiveBossSystem : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UAIWorldDirectorSubsystem* InDirector);
    void AdaptBosses();

    TArray<int32> GetActiveAbilities(int32 BossID) const;

private:
    UPROPERTY()
    UAIWorldDirectorSubsystem* Director;

    TMap<int32, TArray<int32>> ActiveAbilities;

    static constexpr int32 MAX_ADAPTIVE_BOSSES = 5;
};
