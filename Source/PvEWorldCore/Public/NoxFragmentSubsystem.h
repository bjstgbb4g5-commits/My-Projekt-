#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PvETypes.h"
#include "NoxFragmentSubsystem.generated.h"

UCLASS()
class PVEWORLDCORE_API UNoxFragmentSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Nox")
    void GenerateFragments(int32 Count);

    UFUNCTION(BlueprintCallable, Category = "Nox")
    void DiscoverFragment(int32 FragmentID);

protected:
    UPROPERTY()
    TMap<int32, FNoxFragment> Fragments;

    int32 NextFragmentID = 1;
};
