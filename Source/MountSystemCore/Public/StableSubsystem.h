#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MountData.h"
#include "StableSubsystem.generated.h"

UCLASS()
class MOUNTSYSTEMCORE_API UStableSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Stable")
    void HealMount(FMountData& Mount, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Stable")
    float GetHealingCost(const FMountData& Mount) const;

    UFUNCTION(BlueprintCallable, Category = "Stable")
    void HealMountInstant(FMountData& Mount, APlayerController* Player);
};
