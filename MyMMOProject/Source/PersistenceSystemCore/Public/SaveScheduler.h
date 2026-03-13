#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SaveScheduler.generated.h"

UCLASS()
class PERSISTENCESYSTEMCORE_API USaveScheduler : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void StartAutoSave(int32 IntervalSeconds);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void StopAutoSave();

private:
    FTimerHandle AutoSaveTimerHandle;
    bool bSaveInProgress = false;

    void AutoSave();
};
