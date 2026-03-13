#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldEventOrchestrator.generated.h"

class UAIWorldDirectorSubsystem;
class ULiveEventSubsystem;

UCLASS()
class AIDIRECTORCORE_API UWorldEventOrchestrator : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UAIWorldDirectorSubsystem* InDirector);
    void ConsiderEvents();

private:
    UPROPERTY()
    UAIWorldDirectorSubsystem* Director;

    UPROPERTY()
    ULiveEventSubsystem* LiveEvents;

    void TriggerEliteHunt();
    void TriggerCultActivity();
    void TriggerResourceRush();
    void TriggerCaravanDefense();
};
