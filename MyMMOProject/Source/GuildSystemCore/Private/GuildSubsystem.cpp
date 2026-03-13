#include "GuildSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "GuildComponent.h"
#include "Engine/World.h"
#include "PlayerRegistrySubsystem.h"
#include "InventorySubsystem.h"
#include "EconomyCore/Public/PlayerEconomyComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "GuildSaveGame.h" // создадим позже

void UGuildSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    LoadGuilds();
}

void UGuildSubsystem::Deinitialize()
{
    SaveGuilds();
    Guilds.Empty();
    PlayerGuildMap.Empty();
    Super::Deinitialize();
}

bool UGuildSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

int32 UGuildSubsystem::CreateGuild(const FString& GuildName, const FString& Tag, APlayerState* Founder)
{
    if (!IsServer() || !Founder) return -1;

    // Проверка уникальности имени (упрощённо)
    for (const auto& Pair : Guilds)
    {
        if (Pair.Value.GuildName.Equals(GuildName))
            return -1;
    }

    int32 GuildId = NextGuildId++;
    FGuildData Data;
    Data.GuildId = GuildId;
    Data.GuildName = GuildName;
    Data.Tag = Tag;
    Data.LeaderId = Founder->GetUniqueId();
    Data.FoundedTime = FDateTime::UtcNow();

    FGuildMember Leader;
    Leader.PlayerId = Founder->GetUniqueId();
    Leader.Rank = EGuildRank::GuildMaster;
    Leader.JoinedTime = FDateTime::UtcNow();
    Data.Members.Add(Leader);

    Guilds.Add(GuildId, Data);
    PlayerGuildMap.Add(Founder->GetUniqueId(), GuildId);

    // Установить компонент гильдии игроку (если есть)
    APawn* Pawn = Founder->GetPawn();
    if (Pawn)
    {
        if (UGuildComponent* Comp = Pawn->FindComponentByClass<UGuildComponent>())
        {
            Comp->SetGuild(GuildId, EGuildRank::GuildMaster);
        }
    }

    SaveGuilds(); // автосохранение при создании
    return GuildId;
}

const FGuildData* UGuildSubsystem::GetGuild(int32 GuildId) const
{
    return Guilds.Find(GuildId);
}

int32 UGuildSubsystem::GetPlayerGuildId(APlayerState* Player) const
{
    if (!Player) return -1;
    const int32* Found = PlayerGuildMap.Find(Player->GetUniqueId());
    return Found ? *Found : -1;
}

bool UGuildSubsystem::AddMember(int32 GuildId, APlayerState* Player, EGuildRank Rank)
{
    if (!IsServer() || !Player) return false;
    FGuildData* Guild = Guilds.Find(GuildId);
    if (!Guild) return false;

    // Проверка, не состоит ли уже
    if (PlayerGuildMap.Contains(Player->GetUniqueId())) return false;

    FGuildMember Member;
    Member.PlayerId = Player->GetUniqueId();
    Member.Rank = Rank;
    Member.JoinedTime = FDateTime::UtcNow();
    Guild->Members.Add(Member);
    PlayerGuildMap.Add(Player->GetUniqueId(), GuildId);

    APawn* Pawn = Player->GetPawn();
    if (Pawn)
    {
        if (UGuildComponent* Comp = Pawn->FindComponentByClass<UGuildComponent>())
        {
            Comp->SetGuild(GuildId, Rank);
        }
    }

    SaveGuilds();
    return true;
}

bool UGuildSubsystem::RemoveMember(int32 GuildId, APlayerState* Player)
{
    if (!IsServer() || !Player) return false;
    FGuildData* Guild = Guilds.Find(GuildId);
    if (!Guild) return false;

    auto* Member = Guild->Members.FindByPredicate([&](const FGuildMember& M) { return M.PlayerId == Player->GetUniqueId(); });
    if (!Member) return false;

    Guild->Members.RemoveAll([&](const FGuildMember& M) { return M.PlayerId == Player->GetUniqueId(); });
    PlayerGuildMap.Remove(Player->GetUniqueId());

    APawn* Pawn = Player->GetPawn();
    if (Pawn)
    {
        if (UGuildComponent* Comp = Pawn->FindComponentByClass<UGuildComponent>())
        {
            Comp->SetGuild(-1, EGuildRank::Recruit);
        }
    }

    SaveGuilds();
    return true;
}

bool UGuildSubsystem::SetMemberRank(int32 GuildId, APlayerState* Player, EGuildRank NewRank)
{
    if (!IsServer() || !Player) return false;
    FGuildData* Guild = Guilds.Find(GuildId);
    if (!Guild) return false;

    for (FGuildMember& M : Guild->Members)
    {
        if (M.PlayerId == Player->GetUniqueId())
        {
            M.Rank = NewRank;
            APawn* Pawn = Player->GetPawn();
            if (Pawn)
            {
                if (UGuildComponent* Comp = Pawn->FindComponentByClass<UGuildComponent>())
                {
                    Comp->SetGuild(GuildId, NewRank);
                }
            }
            SaveGuilds();
            return true;
        }
    }
    return false;
}

bool UGuildSubsystem::DepositItem(int32 GuildId, APlayerState* Player, int32 ItemID, int32 Quantity)
{
    if (!IsServer() || !Player || Quantity <= 0) return false;
    FGuildData* Guild = Guilds.Find(GuildId);
    if (!Guild) return false;

    // Проверяем, что игрок состоит в гильдии
    if (!PlayerGuildMap.Contains(Player->GetUniqueId()) || PlayerGuildMap[Player->GetUniqueId()] != GuildId)
        return false;

    APawn* Pawn = Player->GetPawn();
    if (!Pawn) return false;

    UPlayerEconomyComponent* Economy = Pawn->FindComponentByClass<UPlayerEconomyComponent>();
    if (!Economy) return false;

    // Используем InventorySubsystem для удаления предмета из инвентаря игрока
    UInventorySubsystem* InvSub = GetWorld()->GetSubsystem<UInventorySubsystem>();
    if (!InvSub) return false;

    // Предположим, что у нас есть метод RemoveItemByID, возвращающий true, если удалось
    if (!Economy->RemoveItem(ItemID, Quantity)) // нужно реализовать в PlayerEconomyComponent
        return false;

    // Добавляем в банк гильдии
    int32& Current = Guild->Bank.FindOrAdd(ItemID);
    Current += Quantity;

    SaveGuilds();
    return true;
}

bool UGuildSubsystem::WithdrawItem(int32 GuildId, APlayerState* Player, int32 ItemID, int32 Quantity)
{
    if (!IsServer() || !Player || Quantity <= 0) return false;
    FGuildData* Guild = Guilds.Find(GuildId);
    if (!Guild) return false;

    if (!PlayerGuildMap.Contains(Player->GetUniqueId()) || PlayerGuildMap[Player->GetUniqueId()] != GuildId)
        return false;

    int32* Current = Guild->Bank.Find(ItemID);
    if (!Current || *Current < Quantity) return false;

    *Current -= Quantity;
    if (*Current <= 0) Guild->Bank.Remove(ItemID);

    // Добавляем предмет игроку
    APawn* Pawn = Player->GetPawn();
    if (!Pawn) return false;

    UPlayerEconomyComponent* Economy = Pawn->FindComponentByClass<UPlayerEconomyComponent>();
    if (!Economy) return false;

    // Используем InventorySubsystem для добавления
    UInventorySubsystem* InvSub = GetWorld()->GetSubsystem<UInventorySubsystem>();
    if (!InvSub) return false;

    FItemInstance Item;
    Item.ItemID = ItemID;
    Item.Quantity = Quantity;
    // MaxStack и другие поля должны быть установлены из ItemSystemCore
    // Пока предположим, что есть способ создать предмет

    // Добавляем в инвентарь
    // InvSub->AddItem(..., Item); // нужно передать инвентарь игрока
    // Упростим: используем Economy->AddItem (должен быть реализован)
    Economy->AddItem(ItemID, Quantity, false); // предполагаем метод

    SaveGuilds();
    return true;
}

void UGuildSubsystem::SaveGuilds()
{
    if (!IsServer()) return;

    // Создаём SaveGame объект
    UGuildSaveGame* SaveGame = Cast<UGuildSaveGame>(UGameplayStatics::CreateSaveGameObject(UGuildSaveGame::StaticClass()));
    if (!SaveGame) return;

    Guilds.GenerateValueArray(SaveGame->Guilds);
    SaveGame->NextGuildId = NextGuildId;

    UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("Guilds"), 0);
}

void UGuildSubsystem::LoadGuilds()
{
    if (!IsServer()) return;

    if (!UGameplayStatics::DoesSaveGameExist(TEXT("Guilds"), 0)) return;

    UGuildSaveGame* SaveGame = Cast<UGuildSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Guilds"), 0));
    if (!SaveGame) return;

    Guilds.Empty();
    for (const FGuildData& Data : SaveGame->Guilds)
    {
        Guilds.Add(Data.GuildId, Data);
    }
    NextGuildId = SaveGame->NextGuildId;

    // Восстанавливаем карту игроков (можно также хранить в сохранении)
    // Здесь придётся перестроить PlayerGuildMap при логине игроков
}
