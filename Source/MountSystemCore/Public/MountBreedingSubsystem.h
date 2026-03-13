#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MountData.h"
#include "MountBreedingSubsystem.generated.h"

UCLASS()
class MOUNTSYSTEMCORE_API UMountBreedingSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Breeding")
    bool CanBreed(const FMountData& ParentA, const FMountData& ParentB) const;

    UFUNCTION(BlueprintCallable, Category = "Breeding")
    FMountData BreedMount(const FMountData& ParentA, const FMountData& ParentB);

    UFUNCTION(BlueprintCallable, Category = "Breeding")
    int32 GetBreedingCost(const FMountData& ParentA, const FMountData& ParentB) const;
};
