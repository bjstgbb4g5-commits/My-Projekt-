#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PvETypes.h"
#include "WorldBossSubsystem.generated.h"

UCLASS()
class PVEWORLDCORE_API UWorldBossSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "WorldBoss")
    void SpawnWorldBoss(int32 BossID, FVector Location);

    UFUNCTION(BlueprintCallable, Category = "WorldBoss")
    void KillWorldBoss(int32 BossID);

    UFUNCTION(BlueprintCallable, Category = "WorldBoss")
    float GetScaledHealth(int32 BossID, int32 PlayersNearby) const;

protected:
    UPROPERTY()
    TMap<int32, FWorldBossData> ActiveBosses;
};
