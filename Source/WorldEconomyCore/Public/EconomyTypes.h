#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "EconomyTypes.generated.h"

UENUM(BlueprintType)
enum class ECurrencyType : uint8
{
    Gold,
    CityMark,
    Resource
};

UENUM(BlueprintType)
enum class EResourceType : uint8
{
    Wood,
    Ore,
    Fiber,
    Crystal,
    Ether
};

UENUM(BlueprintType)
enum class ECraftingType : uint8
{
    Blacksmith,
    Woodwork,
    Alchemy,
    Tailoring,
    ArcaneForge
};

UENUM(BlueprintType)
enum class EContractType : uint8
{
    ProduceGoods,
    HuntCriminal,
    EscortCaravan,
    ExpandCity,
    MilitaryCampaign
};

USTRUCT(BlueprintType)
struct FCityCurrency
{
    GENERATED_BODY()

    UPROPERTY()
    int32 CityID = 0;

    UPROPERTY()
    FString CurrencyName;

    UPROPERTY()
    float ExchangeRateToGold = 1.0f;
};

USTRUCT()
struct FResourceNode
{
    GENERATED_BODY()

    int32 NodeID = 0;
    int32 RegionID = 0;
    EResourceType ResourceType = EResourceType::Wood;
    int32 TotalAmount = 0;
    int32 RemainingAmount = 0;
    int32 Tier = 1;
    float SpawnTime = 0.f;
};

USTRUCT()
struct FMarketOrder
{
    GENERATED_BODY()

    int32 OrderID = 0;
    int32 CityID = 0;
    int32 ItemID = 0;
    int32 Quantity = 0;
    float Price = 0.f;
    bool bIsBuyOrder = false;
    float CreationTime = 0.f;
    TWeakObjectPtr<class APlayerState> Owner;
};

USTRUCT()
struct FItemStack
{
    GENERATED_BODY()

    int32 ItemID = 0;
    int32 Quantity = 0;
    bool bStolen = false;
};

USTRUCT()
struct FCaravanData
{
    GENERATED_BODY()

    int32 CaravanID = 0;
    int32 OriginCity = 0;
    int32 DestinationCity = 0;
    TArray<FItemStack> Cargo;
    float TravelProgress = 0.f;
    bool bIsUnderAttack = false;
};

USTRUCT(BlueprintType)
struct FRecipe
{
    GENERATED_BODY()

    UPROPERTY()
    int32 RecipeID = 0;

    UPROPERTY()
    int32 ResultItemID = 0;

    UPROPERTY()
    TMap<EResourceType, int32> Ingredients;

    UPROPERTY()
    ECraftingType StationType = ECraftingType::Blacksmith;

    UPROPERTY()
    float CraftTime = 5.f;
};

USTRUCT()
struct FEconomyMetrics
{
    GENERATED_BODY()

    float TotalGoldInWorld = 0.f;
    int32 TotalItemsCrafted = 0;
    int32 TotalItemsDestroyed = 0;
    float AveragePriceLevel = 1.0f;
};
