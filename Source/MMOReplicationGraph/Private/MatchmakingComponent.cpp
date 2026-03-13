#include "MatchmakingComponent.h"
#include "Net/UnrealNetwork.h"

UMatchmakingComponent::UMatchmakingComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
    bInQueue = false;
    CurrentQueue = EMatchQueueType::Arena1v1;
}

void UMatchmakingComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UMatchmakingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UMatchmakingComponent, bInQueue, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UMatchmakingComponent, CurrentQueue, COND_OwnerOnly);
}

void UMatchmakingComponent::SetQueueState(bool bQueued, EMatchQueueType QueueType)
{
    if (!GetOwner()->HasAuthority()) return;
    bInQueue = bQueued;
    CurrentQueue = QueueType;
}

void UMatchmakingComponent::Client_MatchFound_Implementation(int32 MatchId, EMatchQueueType QueueType, const TArray<FUniqueNetIdRepl>& TeamPlayers)
{
    // Обновить состояние
    bInQueue = false;
    OnMatchFound.Broadcast(MatchId);
    // Можно также передать информацию о команде в UI
}
