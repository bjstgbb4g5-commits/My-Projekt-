#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HouseTypes.h"
#include "HouseComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HOUSESYSTEMCORE_API UHouseComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHouseComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category = "House")
    void SetHouse(int32 InHouseId, EHouseRank InRank);

    UFUNCTION(BlueprintCallable, Category = "House")
    int32 GetHouseId() const { return HouseId; }

    UFUNCTION(BlueprintCallable, Category = "House")
    EHouseRank GetHouseRank() const { return Rank; }

protected:
    UPROPERTY(Replicated)
    int32 HouseId;

    UPROPERTY(Replicated)
    EHouseRank Rank;
};
