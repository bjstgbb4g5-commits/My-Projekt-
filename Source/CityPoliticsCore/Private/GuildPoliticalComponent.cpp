#include "GuildPoliticalComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"

UGuildPoliticalComponent::UGuildPoliticalComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
    GuildBudget = 0;
}

void UGuildPoliticalComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UGuildPoliticalComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UGuildPoliticalComponent, GuildBudget);
    DOREPLIFETIME(UGuildPoliticalComponent, ControlledDistricts);
    DOREPLIFETIME(UGuildPoliticalComponent, CouncilMembers);
}

bool UGuildPoliticalComponent::TryAddCouncilMember(APlayerState* Player)
{
    if (!GetOwner()->HasAuthority()) return false;
    if (!Player) return false;
    if (IsCouncilFull()) return false;

    for (const FCouncilSeat& Seat : CouncilMembers)
    {
        if (Seat.Member == Player) return false;
    }

    FCouncilSeat NewSeat;
    NewSeat.Member = Player;
    NewSeat.JoinedAt = FDateTime::UtcNow();
    CouncilMembers.Add(NewSeat);
    return true;
}

void UGuildPoliticalComponent::CleanupCouncil()
{
    if (!GetOwner()->HasAuthority()) return;
    CouncilMembers.RemoveAll([](const FCouncilSeat& Seat) {
        return !IsValid(Seat.Member);
    });
}

void UGuildPoliticalComponent::AddGuildBudget(int32 Amount)
{
    if (!GetOwner()->HasAuthority()) return;
    GuildBudget += Amount;
}
