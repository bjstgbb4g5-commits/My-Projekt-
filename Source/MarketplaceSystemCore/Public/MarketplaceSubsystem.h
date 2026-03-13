#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "MarketplaceTypes.h"
#include "MarketplaceSubsystem.generated.h"

class UCityPoliticsSubsystem;

UCLASS()
class MARKETPLACESYSTEMCORE_API UMarketplaceSubsystem
    : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UMarketplaceSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

    // Создание лота на продажу
    UFUNCTION(BlueprintCallable, Category = "Marketplace")
    int32 CreateListing(const FUniqueNetIdRepl& Seller, int32 ItemID, int32 Quantity, int32 Price, EMarketZone City, ECityCurrency Currency);

    // Покупка лота
    UFUNCTION(BlueprintCallable, Category = "Marketplace")
    bool BuyListing(const FUniqueNetIdRepl& Buyer, int32 ListingID);

    // Создание заказа на покупку
    UFUNCTION(BlueprintCallable, Category = "Marketplace")
    int32 CreateOrder(const FUniqueNetIdRepl& Buyer, int32 ItemID, int32 Quantity, int32 Price, EMarketZone City, ECityCurrency Currency);

    // Заполнение заказа на покупку
    UFUNCTION(BlueprintCallable, Category = "Marketplace")
    bool FillOrder(const FUniqueNetIdRepl& Seller, int32 OrderID, int32 Quantity);

    // Получить среднюю цену предмета
    UFUNCTION(BlueprintCallable, Category = "Marketplace")
    int32 GetAveragePrice(int32 ItemID) const;

    // Получить список лотов для города
    UFUNCTION(BlueprintCallable, Category = "Marketplace")
    TArray<FMarketListing> GetListingsForCity(EMarketZone City) const;

    UFUNCTION(BlueprintCallable, Category = "Marketplace")
    TArray<FMarketOrder> GetOrdersForCity(EMarketZone City) const;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    bool IsServer() const;
    void UpdatePriceHistory(int32 ItemID, int32 Price);
    int32 GetCityTax(EMarketZone City) const;
    void LoadState();
    void SaveState();

    UPROPERTY(Replicated)
    TMap<int32, FMarketListing> Listings;

    UPROPERTY(Replicated)
    TMap<int32, FMarketOrder> Orders;

    UPROPERTY()
    TMap<int32, FMarketPriceHistory> PriceHistory;

    int32 NextListingID = 1;
    int32 NextOrderID = 1;

    static constexpr int32 MAX_LISTINGS_PER_PLAYER = 50;
    static constexpr int32 MAX_ORDERS_PER_PLAYER = 30;
    static constexpr FTimespan LISTING_DURATION = FTimespan::FromHours(48);
    static constexpr int32 MAX_ACTIVE_ORDERS = 1000;

    float ExpirationCheckTimer = 0.f;
    static constexpr float EXPIRATION_CHECK_INTERVAL = 60.f;

    FTimerHandle SaveTimerHandle;
};
