#include "PartySubsystem.h"
#include "PartyComponent.h"
#include "GameFramework/PlayerState.h"
#include "Engine/World.h"
#include "PlayerRegistrySubsystem.h"

void UPartySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UPartySubsystem::Deinitialize()
{
    Parties.Empty();
    PlayerPartyMap.Empty();
    PendingInvites.Empty();
    Super::Deinitialize();
}

bool UPartySubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UPartySubsystem::IsPlayerInParty(const FUniqueNetIdRepl& PlayerId) const
{
    return PlayerPartyMap.Contains(PlayerId);
}

int32 UPartySubsystem::CreateParty(APlayerState* Leader)
{
    if (!IsServer() || !Leader) return -1;
    FUniqueNetIdRepl LeaderId = Leader->GetUniqueId();
    if (IsPlayerInParty(LeaderId)) return -1;

    int32 PartyId = NextPartyId++;
    FPartyData Data;
    Data.PartyId = PartyId;
    Data.LeaderId = LeaderId;

    FPartyMemberData Member;
    Member.PlayerId = LeaderId;
    Member.Role = EPartyRole::Leader;
    Data.Members.Add(Member);

    Parties.Add(PartyId, Data);
    PlayerPartyMap.Add(LeaderId, PartyId);

    APawn* Pawn = Leader->GetPawn();
    if (Pawn)
    {
        if (UPartyComponent* Comp = Pawn->FindComponentByClass<UPartyComponent>())
        {
            Comp->SetParty(PartyId, EPartyRole::Leader);
        }
    }

    return PartyId;
}

bool UPartySubsystem::SendPartyInvite(APlayerState* Inviter, APlayerState* Invitee)
{
    if (!IsServer() || !Inviter || !Invitee) return false;
    FUniqueNetIdRepl InviterId = Inviter->GetUniqueId();
    FUniqueNetIdRepl InviteeId = Invitee->GetUniqueId();

    int32* PartyIdPtr = PlayerPartyMap.Find(InviterId);
    if (!PartyIdPtr) return false; // приглашающий не в партии

    FPartyData* Party = Parties.Find(*PartyIdPtr);
    if (!Party) return false;

    // Проверка, что приглашающий лидер или ассистент
    auto* MemberData = Party->Members.FindByPredicate([&](const FPartyMemberData& M) { return M.PlayerId == InviterId; });
    if (!MemberData || (MemberData->Role != EPartyRole::Leader && MemberData->Role != EPartyRole::Assistant))
        return false;

    if (IsPlayerInParty(InviteeId)) return false; // уже в партии

    // Удаляем старые приглашения
    PendingInvites.Remove(InviteeId);

    FPartyInviteData Invite;
    Invite.PartyId = *PartyIdPtr;
    Invite.InviterId = InviterId;
    Invite.ExpireTime = FDateTime::UtcNow() + FTimespan::FromSeconds(InviteTimeoutSeconds);

    PendingInvites.FindOrAdd(InviteeId).Add(Invite);
    return true;
}

bool UPartySubsystem::AcceptPartyInvite(APlayerState* Player, int32 PartyId)
{
    if (!IsServer() || !Player) return false;
    FUniqueNetIdRepl PlayerId = Player->GetUniqueId();

    TArray<FPartyInviteData>* Invites = PendingInvites.Find(PlayerId);
    if (!Invites) return false;

    FDateTime Now = FDateTime::UtcNow();
    auto* Invite = Invites->FindByPredicate([&](const FPartyInviteData& I) { return I.PartyId == PartyId && I.ExpireTime > Now; });
    if (!Invite) return false;

    // Удаляем все приглашения для этого игрока
    PendingInvites.Remove(PlayerId);

    FPartyData* Party = Parties.Find(PartyId);
    if (!Party) return false;

    if (Party->Members.Num() >= MaxPartySize) return false;

    FPartyMemberData NewMember;
    NewMember.PlayerId = PlayerId;
    NewMember.Role = EPartyRole::Member;
    Party->Members.Add(NewMember);
    PlayerPartyMap.Add(PlayerId, PartyId);

    APawn* Pawn = Player->GetPawn();
    if (Pawn)
    {
        if (UPartyComponent* Comp = Pawn->FindComponentByClass<UPartyComponent>())
        {
            Comp->SetParty(PartyId, EPartyRole::Member);
        }
    }

    return true;
}

bool UPartySubsystem::DeclinePartyInvite(APlayerState* Player, int32 PartyId)
{
    if (!IsServer() || !Player) return false;
    FUniqueNetIdRepl PlayerId = Player->GetUniqueId();

    TArray<FPartyInviteData>* Invites = PendingInvites.Find(PlayerId);
    if (!Invites) return false;

    int32 Removed = Invites->RemoveAll([PartyId](const FPartyInviteData& I) { return I.PartyId == PartyId; });
    if (Removed > 0)
    {
        if (Invites->Num() == 0) PendingInvites.Remove(PlayerId);
        return true;
    }
    return false;
}

bool UPartySubsystem::LeaveParty(APlayerState* Player)
{
    if (!IsServer() || !Player) return false;
    FUniqueNetIdRepl PlayerId = Player->GetUniqueId();

    int32* PartyIdPtr = PlayerPartyMap.Find(PlayerId);
    if (!PartyIdPtr) return false;

    FPartyData* Party = Parties.Find(*PartyIdPtr);
    if (!Party) return false;

    Party->Members.RemoveAll([&](const FPartyMemberData& M) { return M.PlayerId == PlayerId; });
    PlayerPartyMap.Remove(PlayerId);

    APawn* Pawn = Player->GetPawn();
    if (Pawn)
    {
        if (UPartyComponent* Comp = Pawn->FindComponentByClass<UPartyComponent>())
        {
            Comp->SetParty(-1, EPartyRole::Member);
        }
    }

    // Если игрок был лидером, передаём лидерство первому в списке
    if (Party->LeaderId == PlayerId && Party->Members.Num() > 0)
    {
        Party->LeaderId = Party->Members[0].PlayerId;
        Party->Members[0].Role = EPartyRole::Leader;
        // Обновить компонент нового лидера
        UPlayerRegistrySubsystem* Registry = GetWorld()->GetSubsystem<UPlayerRegistrySubsystem>();
        if (Registry)
        {
            APlayerController* NewLeaderPC = Registry->GetPlayerById(Party->LeaderId);
            if (NewLeaderPC && NewLeaderPC->GetPawn())
            {
                if (UPartyComponent* Comp = NewLeaderPC->GetPawn()->FindComponentByClass<UPartyComponent>())
                {
                    Comp->SetParty(*PartyIdPtr, EPartyRole::Leader);
                }
            }
        }
    }

    // Если партия пуста, удаляем её
    if (Party->Members.Num() == 0)
    {
        Parties.Remove(*PartyIdPtr);
    }

    return true;
}

bool UPartySubsystem::KickFromParty(APlayerState* Leader, APlayerState* Target)
{
    if (!IsServer() || !Leader || !Target) return false;
    FUniqueNetIdRepl LeaderId = Leader->GetUniqueId();
    FUniqueNetIdRepl TargetId = Target->GetUniqueId();

    int32* PartyIdPtr = PlayerPartyMap.Find(LeaderId);
    if (!PartyIdPtr) return false;

    FPartyData* Party = Parties.Find(*PartyIdPtr);
    if (!Party) return false;

    if (Party->LeaderId != LeaderId) return false;

    return LeaveParty(Target);
}

bool UPartySubsystem::TransferLeadership(APlayerState* CurrentLeader, APlayerState* NewLeader)
{
    if (!IsServer() || !CurrentLeader || !NewLeader) return false;
    FUniqueNetIdRepl CurrentId = CurrentLeader->GetUniqueId();
    FUniqueNetIdRepl NewId = NewLeader->GetUniqueId();

    int32* PartyIdPtr = PlayerPartyMap.Find(CurrentId);
    if (!PartyIdPtr) return false;

    FPartyData* Party = Parties.Find(*PartyIdPtr);
    if (!Party) return false;

    if (Party->LeaderId != CurrentId) return false;

    auto* NewLeaderData = Party->Members.FindByPredicate([&](const FPartyMemberData& M) { return M.PlayerId == NewId; });
    if (!NewLeaderData) return false;

    Party->LeaderId = NewId;
    NewLeaderData->Role = EPartyRole::Leader;

    // Обновить компоненты
    UPlayerRegistrySubsystem* Registry = GetWorld()->GetSubsystem<UPlayerRegistrySubsystem>();
    if (Registry)
    {
        APlayerController* CurrentPC = Registry->GetPlayerById(CurrentId);
        if (CurrentPC && CurrentPC->GetPawn())
        {
            if (UPartyComponent* Comp = CurrentPC->GetPawn()->FindComponentByClass<UPartyComponent>())
            {
                Comp->SetParty(*PartyIdPtr, EPartyRole::Member);
            }
        }
        APlayerController* NewPC = Registry->GetPlayerById(NewId);
        if (NewPC && NewPC->GetPawn())
        {
            if (UPartyComponent* Comp = NewPC->GetPawn()->FindComponentByClass<UPartyComponent>())
            {
                Comp->SetParty(*PartyIdPtr, EPartyRole::Leader);
            }
        }
    }

    return true;
}

TArray<FPartyMemberData> UPartySubsystem::GetPartyMembers(int32 PartyId) const
{
    const FPartyData* Party = Parties.Find(PartyId);
    return Party ? Party->Members : TArray<FPartyMemberData>();
}

int32 UPartySubsystem::GetPlayerPartyId(APlayerState* Player) const
{
    if (!Player) return -1;
    const int32* Found = PlayerPartyMap.Find(Player->GetUniqueId());
    return Found ? *Found : -1;
}
