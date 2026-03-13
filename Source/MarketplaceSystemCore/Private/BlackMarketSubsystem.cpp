#include "BlackMarketSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UBlackMarketSubsystem::UBlackMarketSubsystem()
{
}

void UBlackMarketSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        GenerateDemand();
        GetWorld()->GetTimerManager().SetTimer(DemandTimerHandle, this, &UBlackMarketSubsystem::OnGenerateDemand, 3600.0f, true); // обновление спроса каждый час
    }
}

void UBlackMarketSubsystem::Deinitialize()
{
    if (IsServer())
    {
        GetWorld()->GetTimerManager().ClearTimer(DemandTimerHandle);
    }
    Super::Deinitialize();
}

bool UBlackMarketSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

float UBlackMarketSubsystem::CalculateBlackMarketPrice(float MarketPrice) const
{
    return MarketPrice * FMath::FRandRange(0.5f, 0.8f);
}

bool UBlackMarketSubsystem::BuyStolenItem(const FUniqueNetIdRepl& Buyer, int32 ItemID, float MarketPrice)
{
    if (!IsServer()) return false;
    // Здесь будет интеграция с инвентарём и проверка, что предмет есть в чёрном рынке
    // Пока заглушка
    return true;
}

void UBlackMarketSubsystem::GenerateDemand()
{
    BlackMarketDemand.Empty();
    // Генерируем 5 случайных требований
    for (int32 i = 0; i < 5; ++i)
    {
        FBlackMarketDemand Demand;
        Demand.ItemID = FMath::RandRange(100, 300);
        Demand.NeededAmount = FMath::RandRange(5, 50);
        Demand.RewardPrice = FMath::RandRange(500, 5000);
        BlackMarketDemand.Add(Demand);
    }
}

void UBlackMarketSubsystem::OnGenerateDemand()
{
    GenerateDemand();
}
