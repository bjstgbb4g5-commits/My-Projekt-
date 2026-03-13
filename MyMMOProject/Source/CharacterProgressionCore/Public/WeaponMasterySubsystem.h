#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WeaponMasteryTypes.h"
#include "WeaponMasterySubsystem.generated.h"

UCLASS()
class CHARACTERPROGRESSIONCORE_API UWeaponMasterySubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "Progression|Weapon")
    float GetRequiredXPForLevel(int32 CurrentLevel) const;

private:
    UPROPERTY()
    class UDataTable* MasteryLevelsTable;
};
