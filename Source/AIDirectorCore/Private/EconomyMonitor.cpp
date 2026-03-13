#include "EconomyMonitor.h"
#include "AIWorldDirectorSubsystem.h"
#include "MarketplaceSystemCore/Public/MarketplaceSubsystem.h"

void UEconomyMonitor::Initialize(UAIWorldDirectorSubsystem* InDirector)
{
    Director = InDirector;
    if (Director && Director->GetWorld())
    {
        Marketplace = Director->GetWorld()->GetSubsystem<UMarketplaceSubsystem>();
    }
}

void UEconomyMonitor::UpdateEconomy()
{
    // Заглушка: анализ цен
}

bool UEconomyMonitor::ShouldIncreaseResource(const FString& ResourceName) const
{
    return false; // заглушка
}
