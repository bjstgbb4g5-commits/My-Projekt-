#include "ChatComponent.h"
#include "ChatSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "Misc/DateTime.h"

UChatComponent::UChatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
    bMuted = false;
}

void UChatComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UChatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UChatComponent, bMuted, COND_OwnerOnly);
}

bool UChatComponent::IsServer() const
{
    return GetOwner() && GetOwner()->HasAuthority();
}

void UChatComponent::SetMuted(bool bMute)
{
    if (!IsServer()) return;
    bMuted = bMute;
    if (bMute)
    {
        GetWorld()->GetTimerManager().SetTimer(UnmuteTimerHandle, this, &UChatComponent::OnMuteExpired, MUTE_DURATION_SECONDS, false);
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(UnmuteTimerHandle);
    }
}

void UChatComponent::OnMuteExpired()
{
    if (IsServer())
    {
        bMuted = false;
    }
}

void UChatComponent::AddMessageCount()
{
    FDateTime Now = FDateTime::UtcNow();
    // Удаляем старые записи
    MessageTimestamps.RemoveAll([Now](const FDateTime& T) { return (Now - T).GetTotalSeconds() > INTERVAL_SECONDS; });
    MessageTimestamps.Add(Now);

    if (MessageTimestamps.Num() > MAX_MESSAGES_PER_INTERVAL)
    {
        // Превышен лимит – мут
        SetMuted(true);
        MessageTimestamps.Empty(); // сброс
    }
}

void UChatComponent::Server_SendChatMessage_Implementation(const FChatMessage& Message)
{
    if (!IsServer()) return;

    if (bMuted) return;

    AddMessageCount();
    if (bMuted) return; // если после проверки замутили, не отправляем

    // Передаём сообщение в подсистему
    UChatSubsystem* ChatSys = GetWorld()->GetSubsystem<UChatSubsystem>();
    if (ChatSys)
    {
        ChatSys->RouteMessage(GetOwner(), Message);
    }
}

void UChatComponent::Client_ReceiveChatMessage_Implementation(const FChatMessage& Message)
{
    // На клиенте это будет обработано UI
    // Здесь можно вызвать делегат или событие
}

void UChatComponent::Multicast_LocalChatMessage_Implementation(const FChatMessage& Message)
{
    // Клиенты, получившие это, отобразят сообщение
    // Но лучше использовать Client_ReceiveChatMessage для всех получателей
}
