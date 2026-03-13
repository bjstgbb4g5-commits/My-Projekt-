#include "MarketplaceSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "MarketplaceSaveGame.h"
#include "CityPoliticsCore/Public/CityPoliticsSubsystem.h"
#include "PlayerEconomyComponent.h"
#include "GameFramework/PlayerState.h"

UMarketplaceSubsystem::UMarketplaceSubsystem()
{
}

void UMarketplaceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        LoadState();
        GetWorld()->GetTimerManager().SetTimer(SaveTimerHandle, this, &UMarketplaceSubsystem::SaveState, 300.0f, true); // автосохранение каждые 5 минут
    }
}

void UMarketplaceSubsystem::Deinitialize()
{
    if (IsServer())
    {
        GetWorld()->GetTimerManager().ClearTimer(SaveTimerHandle);
        SaveState();
    }
    Listings.Empty();
    Orders.Empty();
    PriceHistory.Empty();
    Super::Deinitialize();
}

void UMarketplaceSubsystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UMarketplaceSubsystem, Listings);
    DOREPLIFETIME(UMarketplaceSubsystem, Orders);
}

bool UMarketplaceSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UMarketplaceSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UMarketplaceSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UMarketplaceSubsystem, STATGROUP_Tickables);
}

void UMarketplaceSubsystem::Tick(float DeltaTime)
{
    ExpirationCheckTimer += DeltaTime;
    if (ExpirationCheckTimer < EXPIRATION_CHECK_INTERVAL)
        return;
    ExpirationCheckTimer = 0.f;

    FDateTime Now = FDateTime::UtcNow();
    TArray<int32> ToRemove;
    for (const auto& Pair : Listings)
    {
        if (Pair.Value.ExpireTime <= Now)
            ToRemove.Add(Pair.Key);
    }
    for (int32 ID : ToRemove)
    {
        // Возврат предмета продавцу (будет реализовано через инвентарь)
        UE_LOG(LogTemp, Log, TEXT("Listing %d expired, item returned."), ID);
        Listings.Remove(ID);
    }
}

int32 UMarketplaceSubsystem::CreateListing(const FUniqueNetIdRepl& Seller, int32 ItemID, int32 Quantity, int32 Price, EMarketZone City, ECityCurrency Currency)
{
    if (!IsServer() || !Seller.IsValid()) return -1;

    // Проверка цены
    if (Price <= 0 || Price > 1000000) return -1;

    // Проверка количества лотов от продавца
    int32 SellerListings = 0;
    for (const auto& Pair : Listings)
    {
        if (Pair.Value.SellerID == Seller)
            SellerListings++;
    }
    if (SellerListings >= MAX_LISTINGS_PER_PLAYER) return -1;

    // Проверка наличия предмета у продавца (будет позже через инвентарь)
    // Пока пропускаем

    FMarketListing Listing;
    Listing.ListingID = NextListingID++;
    Listing.SellerID = Seller;
    Listing.ItemID = ItemID;
    Listing.Quantity = Quantity;
    Listing.Price = Price;
    Listing.City = City;
    Listing.Currency = Currency;
    Listing.ExpireTime = FDateTime::UtcNow() + LISTING_DURATION;

    Listings.Add(Listing.ListingID, Listing);
    SaveState();
    return Listing.ListingID;
}

bool UMarketplaceSubsystem::BuyListing(const FUniqueNetIdRepl& Buyer, int32 ListingID)
{
    if (!IsServer() || !Buyer.IsValid()) return false;

    FMarketListing* Listing = Listings.Find(ListingID);
    if (!Listing) return false;

    if (FDateTime::UtcNow() >= Listing->ExpireTime)
    {
        Listings.Remove(ListingID);
        return false;
    }

    // Проверка, что покупатель не является продавцом
    if (Listing->SellerID == Buyer) return false;

    // Расчёт налога (зависит от города)
    int32 TaxPercent = GetCityTax(Listing->City);
    int32 TaxAmount = Listing->Price * TaxPercent / 100;
    int32 SellerGets = Listing->Price - TaxAmount;

    // Здесь должна быть интеграция с инвентарём: списать деньги у покупателя, передать предмет
    // и перечислить деньги продавцу
    // Пока просто удалим лот и запишем в историю цен

    UpdatePriceHistory(Listing->ItemID, Listing->Price);
    Listings.Remove(ListingID);
    SaveState();
    return true;
}

int32 UMarketplaceSubsystem::CreateOrder(const FUniqueNetIdRepl& Buyer, int32 ItemID, int32 Quantity, int32 Price, EMarketZone City, ECityCurrency Currency)
{
    if (!IsServer() || !Buyer.IsValid()) return -1;
    if (Quantity <= 0 || Price <= 0) return -1;

    // Проверка количества заказов от покупателя
    int32 BuyerOrders = 0;
    for (const auto& Pair : Orders)
    {
        if (Pair.Value.BuyerID == Buyer)
            BuyerOrders++;
    }
    if (BuyerOrders >= MAX_ORDERS_PER_PLAYER) return -1;

    FMarketOrder Order;
    Order.OrderID = NextOrderID++;
    Order.BuyerID = Buyer;
    Order.ItemID = ItemID;
    Order.Quantity = Quantity;
    Order.Price = Price;
    Order.City = City;
    Order.Currency = Currency;

    Orders.Add(Order.OrderID, Order);
    SaveState();
    return Order.OrderID;
}

bool UMarketplaceSubsystem::FillOrder(const FUniqueNetIdRepl& Seller, int32 OrderID, int32 Quantity)
{
    if (!IsServer() || !Seller.IsValid()) return false;

    FMarketOrder* Order = Orders.Find(OrderID);
    if (!Order) return false;

    int32 AmountToSell = FMath::Min(Quantity, Order->Quantity);
    int32 TotalPrice = AmountToSell * Order->Price;
    int32 TaxPercent = GetCityTax(Order->City);
    int32 TaxAmount = TotalPrice * TaxPercent / 100;
    int32 SellerGets = TotalPrice - TaxAmount;

    // Здесь должна быть интеграция с инвентарём: списать предмет у продавца, передать деньги
    // и отдать предмет покупателю (он уже зарезервировал золото при создании ордера)
    // Для простоты просто уменьшим количество в ордере

    Order->Quantity -= AmountToSell;
    if (Order->Quantity <= 0)
        Orders.Remove(OrderID);

    UpdatePriceHistory(Order->ItemID, Order->Price);
    SaveState();
    return true;
}

int32 UMarketplaceSubsystem::GetAveragePrice(int32 ItemID) const
{
    const FMarketPriceHistory* History = PriceHistory.Find(ItemID);
    return History ? History->AveragePrice : 0;
}

TArray<FMarketListing> UMarketplaceSubsystem::GetListingsForCity(EMarketZone City) const
{
    TArray<FMarketListing> Result;
    for (const auto& Pair : Listings)
    {
        if (Pair.Value.City == City)
            Result.Add(Pair.Value);
    }
    return Result;
}

TArray<FMarketOrder> UMarketplaceSubsystem::GetOrdersForCity(EMarketZone City) const
{
    TArray<FMarketOrder> Result;
    for (const auto& Pair : Orders)
    {
        if (Pair.Value.City == City)
            Result.Add(Pair.Value);
    }
    return Result;
}

void UMarketplaceSubsystem::UpdatePriceHistory(int32 ItemID, int32 Price)
{
    FMarketPriceHistory& History = PriceHistory.FindOrAdd(ItemID);
    History.ItemID = ItemID;
    History.LastPrices.Add(Price);
    if (History.LastPrices.Num() > 20)
        History.LastPrices.RemoveAt(0);

    int32 Sum = 0;
    for (int32 P : History.LastPrices)
        Sum += P;
    History.AveragePrice = Sum / History.LastPrices.Num();
}

int32 UMarketplaceSubsystem::GetCityTax(EMarketZone City) const
{
    // Можно позже брать из CityPoliticsSubsystem
    switch (City)
    {
    case EMarketZone::Aurelion: return 10;
    case EMarketZone::Skyreach: return 5;
    case EMarketZone::Terrahold: return 12;
    case EMarketZone::Tenebris: return 30;
    case EMarketZone::Forgeburg: return 8;
    case EMarketZone::Greenharbor: return 6;
    case EMarketZone::Arcanum: return 7;
    default: return 10;
    }
}

void UMarketplaceSubsystem::LoadState()
{
    if (!IsServer()) return;

    if (!UGameplayStatics::DoesSaveGameExist(TEXT("Marketplace"), 0)) return;

    UMarketplaceSaveGame* Save = Cast<UMarketplaceSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Marketplace"), 0));
    if (!Save) return;

    Listings.Empty();
    for (const FMarketListing& Listing : Save->Listings)
        Listings.Add(Listing.ListingID, Listing);

    Orders.Empty();
    for (const FMarketOrder& Order : Save->Orders)
        Orders.Add(Order.OrderID, Order);

    PriceHistory = Save->PriceHistory;
    NextListingID = Save->NextListingID;
    NextOrderID = Save->NextOrderID;
}

void UMarketplaceSubsystem::SaveState()
{
    if (!IsServer()) return;

    UMarketplaceSaveGame* Save = Cast<UMarketplaceSaveGame>(UGameplayStatics::CreateSaveGameObject(UMarketplaceSaveGame::StaticClass()));
    if (!Save) return;

    Listings.GenerateValueArray(Save->Listings);
    Orders.GenerateValueArray(Save->Orders);
    Save->PriceHistory = PriceHistory;
    Save->NextListingID = NextListingID;
    Save->NextOrderID = NextOrderID;

    UGameplayStatics::SaveGameToSlot(Save, TEXT("Marketplace"), 0);
}
