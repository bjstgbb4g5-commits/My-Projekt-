#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EconomyTypes.h"
#include "TenebrisBlackMarketSubsystem.generated.h"

UCLASS()
class ECONOMYCORE_API UTenebrisBlackMarketSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UTenebrisBlackMarketSubsystem();

    UFUNCTION(BlueprintCallable, Category = "Economy|BlackMarket")
    bool BuyStolenItem(APlayerController* Player, int32 ItemID, float MarketPrice);

    UFUNCTION(BlueprintCallable, Category = "Economy|BlackMarket")
    float CalculateBlackMarketPrice(float MarketPrice) const;

    UFUNCTION(BlueprintCallable, Category = "Economy|BlackMarket")
    void DestroyItem(int32 ItemID);
};
