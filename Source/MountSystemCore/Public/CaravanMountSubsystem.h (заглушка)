#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MountData.h"
#include "CaravanMountSubsystem.generated.h"

USTRUCT()
struct FCaravanMount
{
    GENERATED_BODY()

    int32 CaravanID = -1;
    int32 OriginCity = 0;
    int32 DestinationCity = 0;
    TArray<FMountData> CargoMounts;
    float TravelProgress = 0.f;
};

UCLASS()
class MOUNTSYSTEMCORE_API UCaravanMountSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Caravan")
    int32 CreateCaravan(int32 OriginCity, int32 DestinationCity, const TArray<FMountData>& CargoMounts);

    UFUNCTION(BlueprintCallable, Category = "Caravan")
    void AttackCaravan(int32 CaravanID);

    UFUNCTION(BlueprintCallable, Category = "Caravan")
    void DeliverCaravan(int32 CaravanID);

private:
    TArray<FCaravanMount> ActiveCaravans;
    int32 NextCaravanID = 1;
};
