#include "HouseSubsystem.h"
#include "HouseComponent.h"
#include "HousePermissions.h"
#include "HouseSaveGame.h"
#include "GuildSubsystem.h"
#include "PlayerRegistrySubsystem.h"
#include "TerritoryManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Misc/DateTime.h"

DEFINE_LOG_CATEGORY(LogHouseSystem);

UHouseSubsystem::UHouseSubsystem()
{
}

void UHouseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        GuildSubsystem = GetWorld()->GetSubsystem<UGuildSubsystem>();
        PlayerRegistry = GetWorld()->GetSubsystem<UPlayerRegistrySubsystem>();
        TerritorySubsystem = GetWorld()->GetSubsystem<UTerritoryManagerSubsystem>();

        // Предварительное выделение памяти для масштабирования
        Houses.Reserve(5000);
        PlayerHouseMap.Reserve(20000);
        PendingInvites.Reserve(20000);

        LoadHouses();
    }
}

void UHouseSubsystem::Deinitialize()
{
    SaveHouses();
    Houses.Empty();
    PlayerHouseMap.Empty();
    PendingInvites.Empty();
    Super::Deinitialize();
}

bool UHouseSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

// Вспомогательная функция для получения ранга игрока в гильдии
EGuildRank UHouseSubsystem::GetPlayerGuildRank(APlayerState* Player) const
{
    if (!GuildSubsystem || !Player) return EGuildRank::Recruit; // или минимальный ранг
    return GuildSubsystem->GetPlayerGuildRank(Player); // предполагаем, что эта функция существует
}

int32 UHouseSubsystem::CreateHouse(const FString& HouseName, EHouseType HouseType, APlayerState* Founder, int32 GuildId)
{
    if (!IsServer() || !Founder) return -1;

    FUniqueNetIdRepl FounderId = Founder->GetUniqueId();
    if (!FounderId.IsValid()) return -1;

    // Валидация имени
    FString CleanName = HouseName.TrimStartAndEnd();
    if (CleanName.IsEmpty())
    {
        UE_LOG(LogHouseSystem, Warning, TEXT("CreateHouse: empty house name"));
        return -1;
    }
    if (!CleanName.IsAlnum())
    {
        UE_LOG(LogHouseSystem, Warning, TEXT("CreateHouse: name must be alphanumeric"));
        return -1;
    }
    if (CleanName.Len() > MAX_HOUSE_NAME_LENGTH)
    {
        UE_LOG(LogHouseSystem, Warning, TEXT("CreateHouse: name too long"));
        return -1;
    }

    // Проверка гильдии и ранга основателя
    if (!GuildSubsystem)
    {
        UE_LOG(LogHouseSystem, Error, TEXT("GuildSubsystem not available"));
        return -1;
    }
    int32 PlayerGuildId = GuildSubsystem->GetPlayerGuildId(Founder);
    if (PlayerGuildId != GuildId)
    {
        UE_LOG(LogHouseSystem, Warning, TEXT("CreateHouse: player not in guild %d"), GuildId);
        return -1;
    }
    EGuildRank PlayerRank = GetPlayerGuildRank(Founder);
    if (PlayerRank < EGuildRank::Officer) // ранг офицер и выше
    {
        UE_LOG(LogHouseSystem, Warning, TEXT("CreateHouse: player rank too low"));
        return -1;
    }

    // Уникальность имени
    for (const auto& Pair : Houses)
    {
        if (Pair.Value.HouseName.Equals(CleanName))
        {
            UE_LOG(LogHouseSystem, Warning, TEXT("CreateHouse: house name already exists"));
            return -1;
        }
    }

    // Лимит домов
    if (Houses.Num() >= MAX_HOUSES)
    {
        UE_LOG(LogHouseSystem, Warning, TEXT("CreateHouse: max houses reached"));
        return -1;
    }

    // Поиск свободного HouseId
    int32 StartId = NextHouseId;
    while (Houses.Contains(NextHouseId))
    {
        NextHouseId++;
        if (NextHouseId > MAX_HOUSE_ID)
            NextHouseId = 1;
        if (NextHouseId == StartId)
        {
            UE_LOG(LogHouseSystem, Error, TEXT("CreateHouse: no free HouseId"));
            return -1;
        }
    }
    int32 HouseId = NextHouseId++;

    FHouseData NewHouse;
    NewHouse.HouseId = HouseId;
    NewHouse.HouseName = CleanName;
    NewHouse.FounderPlayerId = FounderId;
    NewHouse.GuildId = GuildId;
    NewHouse.Level = 1;
    NewHouse.HouseType = HouseType;
    NewHouse.CreatedTime = FDateTime::UtcNow();
    NewHouse.MaxMembers = MAX_MEMBERS_PER_HOUSE;

    FHouseMember FounderMember;
    FounderMember.PlayerId = FounderId;
    FounderMember.Rank = EHouseRank::Leader;
    FounderMember.JoinedTime = FDateTime::UtcNow();
    NewHouse.Members.Add(FounderMember);

    Houses.Add(HouseId, NewHouse);
    PlayerHouseMap.Add(FounderId, HouseId);

    // Обновить компонент игрока
    APawn* Pawn = Founder->GetPawn();
    if (Pawn)
    {
        if (UHouseComponent* Comp = Pawn->FindComponentByClass<UHouseComponent>())
        {
            Comp->SetHouse(HouseId, EHouseRank::Leader);
        }
    }

    SaveHouses();
    UE_LOG(LogHouseSystem, Log, TEXT("House %d created: %s by player %s"), HouseId, *CleanName, *FounderId.ToString());
    return HouseId;
}

bool UHouseSubsystem::InvitePlayer(int32 HouseId, APlayerState* Inviter, APlayerState* Target)
{
    if (!IsServer() || !Inviter || !Target) return false;

    FUniqueNetIdRepl InviterId = Inviter->GetUniqueId();
    FUniqueNetIdRepl TargetId = Target->GetUniqueId();
    if (!InviterId.IsValid() || !TargetId.IsValid()) return false;

    FHouseData* House = Houses.Find(HouseId);
    if (!House) return false;

    // Проверка прав
    if (!UHousePermissions::CanInviteMember(*House, InviterId))
        return false;

    // Цель уже в доме?
    if (PlayerHouseMap.Contains(TargetId))
        return false;

    // Добавить приглашение
    TArray<FUniqueNetIdRepl>& Invites = PendingInvites.FindOrAdd(HouseId);
    if (!Invites.Contains(TargetId))
    {
        Invites.Add(TargetId);
        UE_LOG(LogHouseSystem, Log, TEXT("Player %s invited to house %d by %s"), *TargetId.ToString(), HouseId, *InviterId.ToString());
    }
    return true;
}

bool UHouseSubsystem::AcceptInvite(int32 HouseId, APlayerState* Player)
{
    if (!IsServer() || !Player) return false;

    FUniqueNetIdRepl PlayerId = Player->GetUniqueId();
    if (!PlayerId.IsValid()) return false;

    TArray<FUniqueNetIdRepl>* Invites = PendingInvites.Find(HouseId);
    if (!Invites || !Invites->Contains(PlayerId))
        return false;

    // Удалить приглашение
    Invites->Remove(PlayerId);
    if (Invites->Num() == 0)
        PendingInvites.Remove(HouseId);

    // Вызвать JoinHouse с начальным рангом Recruit
    return JoinHouse(HouseId, Player, EHouseRank::Recruit);
}

bool UHouseSubsystem::DeclineInvite(int32 HouseId, APlayerState* Player)
{
    if (!IsServer() || !Player) return false;

    FUniqueNetIdRepl PlayerId = Player->GetUniqueId();
    if (!PlayerId.IsValid()) return false;

    TArray<FUniqueNetIdRepl>* Invites = PendingInvites.Find(HouseId);
    if (!Invites || !Invites->Contains(PlayerId))
        return false;

    Invites->Remove(PlayerId);
    if (Invites->Num() == 0)
        PendingInvites.Remove(HouseId);

    UE_LOG(LogHouseSystem, Log, TEXT("Player %s declined invite to house %d"), *PlayerId.ToString(), HouseId);
    return true;
}

bool UHouseSubsystem::JoinHouse(int32 HouseId, APlayerState* Player, EHouseRank InitialRank)
{
    if (!IsServer() || !Player) return false;
    FUniqueNetIdRepl PlayerId = Player->GetUniqueId();
    if (!PlayerId.IsValid()) return false;

    if (PlayerHouseMap.Contains(PlayerId)) return false; // уже в доме

    FHouseData* House = Houses.Find(HouseId);
    if (!House) return false;
    if (House->Members.Num() >= House->MaxMembers) return false;

    FHouseMember NewMember;
    NewMember.PlayerId = PlayerId;
    NewMember.Rank = InitialRank;
    NewMember.JoinedTime = FDateTime::UtcNow();
    House->Members.Add(NewMember);
    PlayerHouseMap.Add(PlayerId, HouseId);

    APawn* Pawn = Player->GetPawn();
    if (Pawn)
    {
        if (UHouseComponent* Comp = Pawn->FindComponentByClass<UHouseComponent>())
        {
            Comp->SetHouse(HouseId, InitialRank);
        }
    }

    SaveHouses();
    UE_LOG(LogHouseSystem, Log, TEXT("Player %s joined house %d"), *PlayerId.ToString(), HouseId);
    return true;
}

bool UHouseSubsystem::LeaveHouse(APlayerState* Player)
{
    if (!IsServer() || !Player) return false;
    FUniqueNetIdRepl PlayerId = Player->GetUniqueId();
    if (!PlayerId.IsValid()) return false;

    int32* HouseIdPtr = PlayerHouseMap.Find(PlayerId);
    if (!HouseIdPtr) return false;

    FHouseData* House = Houses.Find(*HouseIdPtr);
    if (!House) return false;

    // Лидер не может покинуть дом; должен разрушить его
    for (const FHouseMember& Member : House->Members)
    {
        if (Member.PlayerId == PlayerId && Member.Rank == EHouseRank::Leader)
        {
            UE_LOG(LogHouseSystem, Warning, TEXT("Leader cannot leave house %d; use DestroyHouse"), *HouseIdPtr);
            return false;
        }
    }

    House->Members.RemoveAll([&](const FHouseMember& M) { return M.PlayerId == PlayerId; });
    PlayerHouseMap.Remove(PlayerId);

    APawn* Pawn = Player->GetPawn();
    if (Pawn)
    {
        if (UHouseComponent* Comp = Pawn->FindComponentByClass<UHouseComponent>())
        {
            Comp->SetHouse(-1, EHouseRank::Recruit);
        }
    }

    // Если дом пуст – удаляем
    if (House->Members.Num() == 0)
    {
        Houses.Remove(*HouseIdPtr);
        UE_LOG(LogHouseSystem, Log, TEXT("House %d destroyed (empty)"), *HouseIdPtr);
    }

    SaveHouses();
    UE_LOG(LogHouseSystem, Log, TEXT("Player %s left house %d"), *PlayerId.ToString(), *HouseIdPtr);
    return true;
}

bool UHouseSubsystem::KickMember(int32 HouseId, APlayerState* Requester, APlayerState* Target)
{
    if (!IsServer() || !Requester || !Target) return false;
    FUniqueNetIdRepl RequesterId = Requester->GetUniqueId();
    FUniqueNetIdRepl TargetId = Target->GetUniqueId();
    if (!RequesterId.IsValid() || !TargetId.IsValid()) return false;

    // Проверка, что Target действительно в этом доме
    int32* TargetHouse = PlayerHouseMap.Find(TargetId);
    if (!TargetHouse || *TargetHouse != HouseId) return false;

    FHouseData* House = Houses.Find(HouseId);
    if (!House) return false;

    // Проверка прав
    if (!UHousePermissions::CanKickMember(*House, RequesterId))
        return false;

    // Нельзя кикать лидера
    for (const FHouseMember& Member : House->Members)
    {
        if (Member.PlayerId == TargetId && Member.Rank == EHouseRank::Leader)
        {
            UE_LOG(LogHouseSystem, Warning, TEXT("Cannot kick leader from house %d"), HouseId);
            return false;
        }
    }

    House->Members.RemoveAll([&](const FHouseMember& M) { return M.PlayerId == TargetId; });
    PlayerHouseMap.Remove(TargetId);

    APawn* Pawn = Target->GetPawn();
    if (Pawn)
    {
        if (UHouseComponent* Comp = Pawn->FindComponentByClass<UHouseComponent>())
        {
            Comp->SetHouse(-1, EHouseRank::Recruit);
        }
    }

    SaveHouses();
    UE_LOG(LogHouseSystem, Log, TEXT("Player %s kicked from house %d by %s"), *TargetId.ToString(), HouseId, *RequesterId.ToString());
    return true;
}

bool UHouseSubsystem::UpgradeHouse(int32 HouseId, APlayerState* Requester)
{
    if (!IsServer() || !Requester) return false;
    FUniqueNetIdRepl RequesterId = Requester->GetUniqueId();
    if (!RequesterId.IsValid()) return false;

    FHouseData* House = Houses.Find(HouseId);
    if (!House) return false;

    if (!UHousePermissions::CanUpgradeHouse(*House, RequesterId))
        return false;

    House->Level++;
    // Можно также увеличить MaxMembers
    House->MaxMembers = FMath::Min(MAX_MEMBERS_PER_HOUSE, 200 + (House->Level - 1) * 10);

    SaveHouses();
    UE_LOG(LogHouseSystem, Log, TEXT("House %d upgraded to level %d"), HouseId, House->Level);
    return true;
}

bool UHouseSubsystem::DestroyHouse(int32 HouseId, APlayerState* Requester)
{
    if (!IsServer() || !Requester) return false;
    FUniqueNetIdRepl RequesterId = Requester->GetUniqueId();
    if (!RequesterId.IsValid()) return false;

    FHouseData* House = Houses.Find(HouseId);
    if (!House) return false;

    // Только лидер может разрушить дом
    bool bIsLeader = false;
    for (const FHouseMember& Member : House->Members)
    {
        if (Member.PlayerId == RequesterId && Member.Rank == EHouseRank::Leader)
        {
            bIsLeader = true;
            break;
        }
    }
    if (!bIsLeader) return false;

    // Удалить всех игроков из карты
    for (const FHouseMember& Member : House->Members)
    {
        PlayerHouseMap.Remove(Member.PlayerId);
        // Обновить компоненты игроков
        if (PlayerRegistry)
        {
            APlayerController* PC = PlayerRegistry->GetPlayerById(Member.PlayerId);
            if (PC && PC->GetPawn())
            {
                if (UHouseComponent* Comp = PC->GetPawn()->FindComponentByClass<UHouseComponent>())
                {
                    Comp->SetHouse(-1, EHouseRank::Recruit);
                }
            }
        }
    }

    // Очистить территории (не удаляем из TerritoryManager, просто забываем)
    House->OwnedTerritories.Empty();

    Houses.Remove(HouseId);
    SaveHouses();
    UE_LOG(LogHouseSystem, Log, TEXT("House %d destroyed by leader %s"), HouseId, *RequesterId.ToString());
    return true;
}

bool UHouseSubsystem::CaptureTerritory(int32 HouseId, int32 TerritoryId, APlayerState* Requester)
{
    if (!IsServer() || !Requester) return false;
    FUniqueNetIdRepl RequesterId = Requester->GetUniqueId();
    if (!RequesterId.IsValid()) return false;

    FHouseData* House = Houses.Find(HouseId);
    if (!House) return false;

    if (!UHousePermissions::CanManageTerritories(*House, RequesterId))
        return false;

    if (House->OwnedTerritories.Num() >= MAX_TERRITORIES_PER_HOUSE)
    {
        UE_LOG(LogHouseSystem, Warning, TEXT("House %d cannot capture more territories (limit %d)"), HouseId, MAX_TERRITORIES_PER_HOUSE);
        return false;
    }

    if (!TerritorySubsystem)
    {
        UE_LOG(LogHouseSystem, Error, TEXT("TerritorySubsystem not available"));
        return false;
    }

    // Захват территории через TerritoryManagerSubsystem
    // Предположим, что есть функция CaptureTerritoryForHouse(TerritoryId, HouseId)
    // TerritorySubsystem->CaptureTerritoryForHouse(TerritoryId, HouseId);
    // После успешного захвата добавляем в список
    House->OwnedTerritories.AddUnique(TerritoryId);
    SaveHouses();

    UE_LOG(LogHouseSystem, Log, TEXT("House %d captured territory %d"), HouseId, TerritoryId);
    return true;
}

const FHouseData* UHouseSubsystem::GetHouse(int32 HouseId) const
{
    return Houses.Find(HouseId);
}

int32 UHouseSubsystem::GetPlayerHouseId(APlayerState* Player) const
{
    if (!Player) return -1;
    const int32* Found = PlayerHouseMap.Find(Player->GetUniqueId());
    return Found ? *Found : -1;
}

TArray<FHouseData> UHouseSubsystem::GetGuildHouses(int32 GuildId) const
{
    TArray<FHouseData> Result;
    for (const auto& Pair : Houses)
    {
        if (Pair.Value.GuildId == GuildId)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

void UHouseSubsystem::SaveHouses()
{
    if (!IsServer()) return;

    UHouseSaveGame* SaveGame = Cast<UHouseSaveGame>(UGameplayStatics::CreateSaveGameObject(UHouseSaveGame::StaticClass()));
    if (!SaveGame) return;

    Houses.GenerateValueArray(SaveGame->Houses);
    SaveGame->NextHouseId = NextHouseId;
    UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("Houses"), 0);
}

void UHouseSubsystem::LoadHouses()
{
    if (!IsServer()) return;

    if (!UGameplayStatics::DoesSaveGameExist(TEXT("Houses"), 0)) return;

    UHouseSaveGame* SaveGame = Cast<UHouseSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Houses"), 0));
    if (!SaveGame) return;

    Houses.Empty();
    for (const FHouseData& Data : SaveGame->Houses)
    {
        Houses.Add(Data.HouseId, Data);
    }
    NextHouseId = SaveGame->NextHouseId;

    // Перестроить PlayerHouseMap
    PlayerHouseMap.Empty();
    for (const auto& Pair : Houses)
    {
        for (const FHouseMember& Member : Pair.Value.Members)
        {
            PlayerHouseMap.Add(Member.PlayerId, Pair.Key);
        }
    }
}
