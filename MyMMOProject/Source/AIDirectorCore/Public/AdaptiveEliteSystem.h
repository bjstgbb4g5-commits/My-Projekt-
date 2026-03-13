#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIDirectorTypes.h"
#include "AdaptiveEliteSystem.generated.h"

class UAIWorldDirectorSubsystem;

UCLASS()
class AIDIRECTORCORE_API UAdaptiveEliteSystem : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UAIWorldDirectorSubsystem* InDirector);
    void AdaptElites();

    TArray<EEliteModule> ChooseModulesForElite(int32 EliteID) const;

private:
    UPROPERTY()
    UAIWorldDirectorSubsystem* Director;

    TMap<int32, TArray<EEliteModule>> EliteModules;

    static constexpr int32 MAX_ELITE_MOBS = 40;
};
