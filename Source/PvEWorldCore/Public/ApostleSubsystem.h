#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PvETypes.h"
#include "ApostleSubsystem.generated.h"

UCLASS()
class PVEWORLDCORE_API UApostleSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Apostle")
    void SpawnApostle(EApostleType Apostle, FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Apostle")
    void KillApostle(EApostleType Apostle);

protected:
    UPROPERTY()
    TMap<EApostleType, FApostleData> ActiveApostles;
};
