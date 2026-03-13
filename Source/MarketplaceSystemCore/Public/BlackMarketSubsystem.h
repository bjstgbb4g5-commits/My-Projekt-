#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MarketplaceTypes.h"
#include "BlackMarketSubsystem.generated.h"

UCLASS()
class MARKETPLACESYSTEMCORE_API UBlackMarketSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UBlackMarketSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "BlackMarket")
    bool BuyStolenItem(const FUniqueNetIdRepl& Buyer, int32 ItemID, float MarketPrice);

    UFUNCTION(BlueprintCallable, Category = "BlackMarket")
    float CalculateBlackMarketPrice(float MarketPrice) const;

    UFUNCTION(BlueprintCallable, Category = "BlackMarket")
    void GenerateDemand();

    UFUNCTION(BlueprintCallable, Category = "BlackMarket")
    TArray<FBlackMarketDemand> GetCurrentDemand() const { return BlackMarketDemand; }

private:
    bool IsServer() const;
    void OnGenerateDemand();

    UPROPERTY()
    TArray<FBlackMarketDemand> BlackMarketDemand;

    FTimerHandle DemandTimerHandle;
};
