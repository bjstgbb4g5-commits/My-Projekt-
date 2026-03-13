#include "TenebrisBlackMarketSubsystem.h"
#include "PlayerEconomyComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

UTenebrisBlackMarketSubsystem::UTenebrisBlackMarketSubsystem()
{
}

float UTenebrisBlackMarketSubsystem::CalculateBlackMarketPrice(float MarketPrice) const
{
    return MarketPrice * FMath::FRandRange(0.6f, 0.8f);
}

bool UTenebrisBlackMarketSubsystem::BuyStolenItem(APlayerController* Player, int32 ItemID, float MarketPrice)
{
    if (!Player) return false;
    UPlayerEconomyComponent* Economy = Player->GetPawn() ? Player->GetPawn()->FindComponentByClass<UPlayerEconomyComponent>() : nullptr;
    if (!Economy) return false;

    float Price = CalculateBlackMarketPrice(MarketPrice);
    if (!Economy->HasGold(Price)) return false;

    Economy->RemoveGold(Price);
    Economy->AddItem(ItemID, 1, true);
    return true;
}

void UTenebrisBlackMarketSubsystem::DestroyItem(int32 ItemID)
{
    // Заглушка
}
