#include "GuildComponent.h"
#include "Net/UnrealNetwork.h"

UGuildComponent::UGuildComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
    GuildId = -1;
    Rank = EGuildRank::Recruit;
}

void UGuildComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UGuildComponent, GuildId, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UGuildComponent, Rank, COND_OwnerOnly);
}

void UGuildComponent::SetGuild(int32 InGuildId, EGuildRank InRank)
{
    if (!GetOwner()->HasAuthority()) return;
    GuildId = InGuildId;
    Rank = InRank;
}

bool UGuildComponent::HasPermission(EGuildPermission Permission) const
{
    // Упрощённо: лидер и гилдмастер имеют все права
    if (Rank == EGuildRank::Leader || Rank == EGuildRank::GuildMaster) return true;
    // Остальным – по настройкам ранга (пока заглушка)
    return false;
}
