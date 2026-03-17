#include "CityMarketSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "PlayerEconomyComponent.h"

UCityMarketSubsystem::UCityMarketSubsystem()
{
}

void UCityMarketSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        ScheduleExecution(60.0f); // каждую минуту
    }
}

void UCityMarketSubsystem::Deinitialize()
{
    if (IsServer())
    {
        GetWorld()->GetTimerManager().ClearTimer(ExecutionTimerHandle);
    }
    SellOrders.Empty();
    BuyOrders.Empty();
    Super::Deinitialize();
}

bool UCityMarketSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

void UCityMarketSubsystem::ScheduleExecution(float IntervalSeconds)
{
    GetWorld()->GetTimerManager().SetTimer(ExecutionTimerHandle, this, &UCityMarketSubsystem::OnExecutionTimer, IntervalSeconds, true);
}

int32 UCityMarketSubsystem::CreateSellOrder(int32 CityID, int32 ItemID, int32 Quantity, float Price, APlayerController* Seller)
{
    if (!IsServer()) return -1;
    if (!Seller) return -1;
    UPlayerEconomyComponent* Economy = Seller->GetPawn() ? Seller->GetPawn()->FindComponentByClass<UPlayerEconomyComponent>() : nullptr;
    if (!Economy || !Economy->HasItem(ItemID, Quantity)) return -1;

    // Списать предмет
    Economy->RemoveItem(ItemID, Quantity);

    FMarketOrder Order;
    Order.OrderID = NextOrderID++;
    Order.CityID = CityID;
    Order.ItemID = ItemID;
    Order.Quantity = Quantity;
    Order.Price = Price;
    Order.bIsBuyOrder = false;
    Order.CreationTime = GetWorld()->GetTimeSeconds();
    Order.Owner = Seller->PlayerState;
    SellOrders.Add(Order);
    return Order.OrderID;
}

int32 UCityMarketSubsystem::CreateBuyOrder(int32 CityID, int32 ItemID, int32 Quantity, float Price, APlayerController* Buyer)
{
    if (!IsServer()) return -1;
    if (!Buyer) return -1;
    UPlayerEconomyComponent* Economy = Buyer->GetPawn() ? Buyer->GetPawn()->FindComponentByClass<UPlayerEconomyComponent>() : nullptr;
    if (!Economy) return -1;
    float Total = Price * Quantity;
    if (!Economy->HasGold(Total)) return -1;

    // Зарезервировать золото
    Economy->RemoveGold(Total);

    FMarketOrder Order;
    Order.OrderID = NextOrderID++;
    Order.CityID = CityID;
    Order.ItemID = ItemID;
    Order.Quantity = Quantity;
    Order.Price = Price;
    Order.bIsBuyOrder = true;
    Order.CreationTime = GetWorld()->GetTimeSeconds();
    Order.Owner = Buyer->PlayerState;
    BuyOrders.Add(Order);
    return Order.OrderID;
}

bool UCityMarketSubsystem::CancelOrder(int32 OrderID)
{
    if (!IsServer()) return false;
    for (int32 i = 0; i < SellOrders.Num(); ++i)
    {
        if (SellOrders[i].OrderID == OrderID)
        {
            // Вернуть предмет
            if (SellOrders[i].Owner.IsValid())
            {
                // найти игрока и добавить предмет
            }
            SellOrders.RemoveAt(i);
            return true;
        }
    }
    for (int32 i = 0; i < BuyOrders.Num(); ++i)
    {
        if (BuyOrders[i].OrderID == OrderID)
        {
            // Вернуть золото
            BuyOrders.RemoveAt(i);
            return true;
        }
    }
    return false;
}

void UCityMarketSubsystem::ExecuteTrade()
{
    if (!IsServer()) return;
    for (int32 i = SellOrders.Num() - 1; i >= 0; --i)
    {
        FMarketOrder& Sell = SellOrders[i];
        for (int32 j = BuyOrders.Num() - 1; j >= 0; --j)
        {
            FMarketOrder& Buy = BuyOrders[j];
            if (Sell.CityID != Buy.CityID) continue;
            if (Sell.ItemID != Buy.ItemID) continue;
            if (Sell.Price > Buy.Price) continue;
            if (Sell.Quantity <= 0 || Buy.Quantity <= 0) continue;

            int32 TradeQty = FMath::Min(Sell.Quantity, Buy.Quantity);
            float TradePrice = Buy.Price;

            // Передать предмет покупателю, золото продавцу
            // ... (опущено для краткости)

            Sell.Quantity -= TradeQty;
            Buy.Quantity -= TradeQty;

            if (Sell.Quantity <= 0) SellOrders.RemoveAt(i);
            if (Buy.Quantity <= 0) BuyOrders.RemoveAt(j);
            break;
        }
    }
}

void UCityMarketSubsystem::OnExecutionTimer()
{
    ExecuteTrade();
}
