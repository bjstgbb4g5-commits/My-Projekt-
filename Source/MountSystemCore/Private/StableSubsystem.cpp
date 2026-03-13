#include "StableSubsystem.h"
#include "PlayerEconomyComponent.h"

void UStableSubsystem::HealMount(FMountData& Mount, float Amount)
{
    Mount.CurrentHealth = FMath::Clamp(Mount.CurrentHealth + Amount, 0.f, Mount.Stats.MaxHealth);
}

float UStableSubsystem::GetHealingCost(const FMountData& Mount) const
{
    float MissingHP = Mount.Stats.MaxHealth - Mount.CurrentHealth;
    int32 RarityMultiplier = 1;
    switch (Mount.Rarity)
    {
    case EMountRarity::Rare: RarityMultiplier = 2; break;
    case EMountRarity::Epic: RarityMultiplier = 5; break;
    case EMountRarity::Legendary: RarityMultiplier = 10; break;
    case EMountRarity::Mythic: RarityMultiplier = 20; break;
    default: break;
    }
    return MissingHP * RarityMultiplier;
}

void UStableSubsystem::HealMountInstant(FMountData& Mount, APlayerController* Player)
{
    if (!Player) return;
    UPlayerEconomyComponent* Economy = Player->GetPawn()->FindComponentByClass<UPlayerEconomyComponent>();
    if (!Economy) return;
    float Cost = GetHealingCost(Mount);
    if (Economy->RemoveGold(Cost))
    {
        Mount.CurrentHealth = Mount.Stats.MaxHealth;
    }
}
