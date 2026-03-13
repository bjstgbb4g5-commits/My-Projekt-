#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "MarketplaceTypes.generated.h"

UENUM(BlueprintType)
enum class EMarketZone : uint8
{
    Aurelion,
    Skyreach,
    Terrahold,
    Tenebris,
    Forgeburg,
    Greenharbor,
    Arcanum
};

UENUM(BlueprintType)
enum class ECityCurrency : uint8
{
    Aurels,
    SkyMarks,
    TerraTokens,
    ShadowCoins,
    FlameMarks,
    TideCoins,
    ArcaneCrystals
};

USTRUCT(BlueprintType)
struct FMarketListing
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ListingID = -1;

    UPROPERTY()
    int32 ItemID = -1;

    UPROPERTY()
    FUniqueNetIdRepl SellerID;

    UPROPERTY()
    int32 Quantity = 1;

    UPROPERTY()
    int32 Price = 0;

    UPROPERTY()
    ECityCurrency Currency = ECityCurrency::Aurels;

    UPROPERTY()
    EMarketZone City = EMarketZone::Aurelion;

    UPROPERTY()
    FDateTime ExpireTime;
};

USTRUCT(BlueprintType)
struct FMarketOrder
{
    GENERATED_BODY()

    UPROPERTY()
    int32 OrderID = -1;

    UPROPERTY()
    int32 ItemID = -1;

    UPROPERTY()
    FUniqueNetIdRepl BuyerID;

    UPROPERTY()
    int32 Quantity = 1;

    UPROPERTY()
    int32 Price = 0;

    UPROPERTY()
    EMarketZone City = EMarketZone::Aurelion;

    UPROPERTY()
    ECityCurrency Currency = ECityCurrency::Aurels;
};

USTRUCT()
struct FMarketPriceHistory
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ItemID = -1;

    UPROPERTY()
    TArray<int32> LastPrices;

    UPROPERTY()
    int32 AveragePrice = 0;
};

USTRUCT()
struct FBlackMarketDemand
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ItemID = -1;

    UPROPERTY()
    int32 NeededAmount = 0;

    UPROPERTY()
    int32 RewardPrice = 0;
};
