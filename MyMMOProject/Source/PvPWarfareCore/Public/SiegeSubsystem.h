#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PvPTypes.h"
#include "SiegeSubsystem.generated.h"

UCLASS()
class PVPWARFARECORE_API USiegeSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Siege")
    bool CanSpawnSiegeMachine(ESiegeMachineType Type, int32 BattleID) const;

    UFUNCTION(BlueprintCallable, Category = "Siege")
    void RegisterSiegeMachine(ESiegeMachineType Type, int32 BattleID);

    UFUNCTION(BlueprintCallable, Category = "Siege")
    void UnregisterSiegeMachine(ESiegeMachineType Type, int32 BattleID);

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
    // Лимиты для каждого боя (можно загружать из конфига)
    TMap<int32, FSiegeLimitConfig> BattleLimits;
    TMap<int32, TMap<ESiegeMachineType, int32>> BattleMachineCounts;
};
