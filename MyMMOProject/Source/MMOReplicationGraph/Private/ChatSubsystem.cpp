#include "ChatSubsystem.h"
#include "ChatComponent.h"
#include "PlayerRegistrySubsystem.h"
#include "PartySystemCore/Public/PartySubsystem.h"
#include "GuildSystemCore/Public/GuildSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Engine/World.h"

void UChatSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        PlayerRegistry = GetWorld()->GetSubsystem<UPlayerRegistrySubsystem>();
        PartySubsystem = GetWorld()->GetSubsystem<UPartySubsystem>();
        GuildSubsystem = GetWorld()->GetSubsystem<UGuildSubsystem>();
    }
}

void UChatSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

bool UChatSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

UChatComponent* UChatSubsystem::GetChatComponent(APlayerController* PC) const
{
    if (!PC || !PC->GetPawn()) return nullptr;
    return PC->GetPawn()->FindComponentByClass<UChatComponent>();
}

void UChatSubsystem::RouteMessage(AActor* Sender, const FChatMessage& Message)
{
    if (!IsServer()) return;

    switch (Message.Channel)
    {
    case EChatChannel::Local:
        SendLocal(Sender, Message);
        break;
    case EChatChannel::Party:
        SendParty(Sender, Message);
        break;
    case EChatChannel::Guild:
        SendGuild(Sender, Message);
        break;
    case EChatChannel::Trade:
        SendTrade(Message);
        break;
    case EChatChannel::Global:
        SendGlobal(Message);
        break;
    case EChatChannel::Whisper:
        SendWhisper(Sender, Message);
        break;
    case EChatChannel::System:
        // Только сервер может отправлять системные сообщения
        break;
    }
}

void UChatSubsystem::SendLocal(AActor* Sender, const FChatMessage& Message)
{
    const float LocalRange = 3000.f; // 30 метров в сантиметрах
    FVector SenderLocation = Sender->GetActorLocation();

    TArray<APlayerController*> AllPlayers = PlayerRegistry->GetAllPlayers();
    for (APlayerController* PC : AllPlayers)
    {
        if (!PC || !PC->GetPawn()) continue;
        float Dist = FVector::Dist(SenderLocation, PC->GetPawn()->GetActorLocation());
        if (Dist <= LocalRange)
        {
            UChatComponent* Comp = GetChatComponent(PC);
            if (Comp)
            {
                Comp->Client_ReceiveChatMessage(Message);
            }
        }
    }
}

void UChatSubsystem::SendParty(AActor* Sender, const FChatMessage& Message)
{
    APlayerController* SenderPC = Cast<APlayerController>(Sender);
    if (!SenderPC) return;
    APlayerState* PS = SenderPC->PlayerState;
    if (!PS) return;

    int32 PartyId = PartySubsystem->GetPlayerPartyId(PS);
    if (PartyId == -1) return;

    TArray<FPartyMemberData> Members = PartySubsystem->GetPartyMembers(PartyId);
    for (const FPartyMemberData& Member : Members)
    {
        APlayerController* MemberPC = PlayerRegistry->GetPlayerById(Member.PlayerId);
        if (!MemberPC) continue;
        UChatComponent* Comp = GetChatComponent(MemberPC);
        if (Comp)
        {
            Comp->Client_ReceiveChatMessage(Message);
        }
    }
}

void UChatSubsystem::SendGuild(AActor* Sender, const FChatMessage& Message)
{
    APlayerController* SenderPC = Cast<APlayerController>(Sender);
    if (!SenderPC) return;
    APlayerState* PS = SenderPC->PlayerState;
    if (!PS) return;

    int32 GuildId = GuildSubsystem->GetPlayerGuildId(PS);
    if (GuildId == -1) return;

    const FGuildData* Guild = GuildSubsystem->GetGuild(GuildId);
    if (!Guild) return;

    for (const FGuildMember& Member : Guild->Members)
    {
        APlayerController* MemberPC = PlayerRegistry->GetPlayerById(Member.PlayerId);
        if (!MemberPC) continue;
        UChatComponent* Comp = GetChatComponent(MemberPC);
        if (Comp)
        {
            Comp->Client_ReceiveChatMessage(Message);
        }
    }
}

void UChatSubsystem::SendTrade(const FChatMessage& Message)
{
    // Торговый канал – всем игрокам (можно отправить всем)
    TArray<APlayerController*> AllPlayers = PlayerRegistry->GetAllPlayers();
    for (APlayerController* PC : AllPlayers)
    {
        UChatComponent* Comp = GetChatComponent(PC);
        if (Comp)
        {
            Comp->Client_ReceiveChatMessage(Message);
        }
    }
}

void UChatSubsystem::SendGlobal(const FChatMessage& Message)
{
    TArray<APlayerController*> AllPlayers = PlayerRegistry->GetAllPlayers();
    for (APlayerController* PC : AllPlayers)
    {
        UChatComponent* Comp = GetChatComponent(PC);
        if (Comp)
        {
            Comp->Client_ReceiveChatMessage(Message);
        }
    }
}

void UChatSubsystem::SendWhisper(AActor* Sender, const FChatMessage& Message)
{
    if (!Message.TargetPlayerId.IsValid()) return;

    APlayerController* TargetPC = PlayerRegistry->GetPlayerById(Message.TargetPlayerId);
    if (!TargetPC) return;

    UChatComponent* TargetComp = GetChatComponent(TargetPC);
    if (TargetComp)
    {
        TargetComp->Client_ReceiveChatMessage(Message);
    }

    // Также отправить копию отправителю (для подтверждения)
    if (Sender)
    {
        UChatComponent* SenderComp = GetChatComponent(Cast<APlayerController>(Sender));
        if (SenderComp)
        {
            SenderComp->Client_ReceiveChatMessage(Message);
        }
    }
}

void UChatSubsystem::BroadcastSystemMessage(const FString& Text)
{
    if (!IsServer()) return;

    FChatMessage SystemMsg;
    SystemMsg.SenderId = FUniqueNetIdRepl();
    SystemMsg.SenderName = TEXT("System");
    SystemMsg.Channel = EChatChannel::System;
    SystemMsg.MessageText = Text;
    SystemMsg.Timestamp = FDateTime::UtcNow();

    TArray<APlayerController*> AllPlayers = PlayerRegistry->GetAllPlayers();
    for (APlayerController* PC : AllPlayers)
    {
        UChatComponent* Comp = GetChatComponent(PC);
        if (Comp)
        {
            Comp->Client_ReceiveChatMessage(SystemMsg);
        }
    }
}
