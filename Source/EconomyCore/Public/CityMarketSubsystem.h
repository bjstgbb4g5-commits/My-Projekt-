#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EconomyTypes.h"
#include "CityMarketSubsystem.generated.h"

UCLASS()
class ECONOMYCORE_API UCityMarketSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UCityMarketSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Economy|Market")
    int32 CreateSellOrder(int32 CityID, int32 ItemID, int32 Quantity, float Price, APlayerController* Seller);

    UFUNCTION(BlueprintCallable, Category = "Economy|Market")
    int32 CreateBuyOrder(int32 CityID, int32 ItemID, int32 Quantity, float Price, APlayerController* Buyer);

    UFUNCTION(BlueprintCallable, Category = "Economy|Market")
    bool CancelOrder(int32 OrderID);

    UFUNCTION(BlueprintCallable, Category = "Economy|Market")
    void ExecuteTrade();

    static constexpr int32 MAX_ACTIVE_ORDERS = 1000;

private:
    bool IsServer() const;
    void ScheduleExecution(float IntervalSeconds);
    void OnExecutionTimer();

    FTimerHandle ExecutionTimerHandle;

    UPROPERTY()
    TArray<FMarketOrder> SellOrders;

    UPROPERTY()
    TArray<FMarketOrder> BuyOrders;

    int32 NextOrderID = 1;
};
