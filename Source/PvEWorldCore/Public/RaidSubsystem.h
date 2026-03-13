#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PvETypes.h"
#include "RaidSubsystem.generated.h"

UCLASS()
class PVEWORLDCORE_API URaidSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Raid")
    int32 CreateRaid(const TArray<APlayerState*>& Players, ERaidDifficulty Difficulty);

    UFUNCTION(BlueprintCallable, Category = "Raid")
    void StartRaid(int32 RaidID);

    UFUNCTION(BlueprintCallable, Category = "Raid")
    void BossKilled(int32 RaidID);

protected:
    UPROPERTY()
    TMap<int32, FRaidInstance> ActiveRaids;

    int32 NextRaidID = 1;
};
