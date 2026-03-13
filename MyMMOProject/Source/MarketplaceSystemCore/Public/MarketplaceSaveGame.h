#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MarketplaceTypes.h"
#include "MarketplaceSaveGame.generated.h"

UCLASS()
class MARKETPLACESYSTEMCORE_API UMarketplaceSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<FMarketListing> Listings;

    UPROPERTY()
    TArray<FMarketOrder> Orders;

    UPROPERTY()
    TMap<int32, FMarketPriceHistory> PriceHistory;

    UPROPERTY()
    int32 NextListingID = 1;

    UPROPERTY()
    int32 NextOrderID = 1;
};
