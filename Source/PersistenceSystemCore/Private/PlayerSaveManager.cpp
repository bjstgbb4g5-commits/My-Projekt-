#include "PlayerSaveManager.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "CharacterProgressionCore/Public/PlayerProgressionComponent.h"
#include "InventorySystemCore/Public/PlayerInventoryActor.h"
#include "EconomyCore/Public/PlayerEconomyComponent.h"
#include "MountSystemCore/Public/PlayerMountComponent.h"
#include "GuildSystemCore/Public/GuildComponent.h"
#include "PlayerSaveGame.h"

void UPlayerSaveManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UPlayerSaveManager::Deinitialize()
{
    if (IsServer())
    {
        SaveAllPlayers();
    }
    Super::Deinitialize();
}

bool UPlayerSaveManager::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

FString UPlayerSaveManager::GetPlayerSaveSlot(const FUniqueNetIdRepl& PlayerId) const
{
    return FString::Printf(TEXT("Player_%s"), *PlayerId.ToString());
}

void UPlayerSaveManager::SavePlayer(APlayerState* Player)
{
    if (!IsServer() || !Player) return;

    FPlayerSaveData SaveData;
    SaveData.PlayerId = Player->GetUniqueId();

    APawn* Pawn = Player->GetPawn();
    if (!Pawn) return; // ничего не сохраняем, если нет персонажа

    // Progression
    if (UPlayerProgressionComponent* Prog = Pawn->FindComponentByClass<UPlayerProgressionComponent>())
    {
        // Здесь нужно заполнить SaveData.WeaponMastery и другие поля
        // Prog->GetWeaponMastery(SaveData.WeaponMastery); // пример
    }

    // Economy
    if (UPlayerEconomyComponent* Econ = Pawn->FindComponentByClass<UPlayerEconomyComponent>())
    {
        SaveData.Gold = Econ->Gold;
        // Также нужно сохранять ресурсы и предметы
    }

    // Mounts
    if (UPlayerMountComponent* Mounts = Pawn->FindComponentByClass<UPlayerMountComponent>())
    {
        // Mounts->GetOwnedMountIDs(SaveData.OwnedMountIDs);
    }

    // Guild
    if (UGuildComponent* Guild = Pawn->FindComponentByClass<UGuildComponent>())
    {
        SaveData.GuildId = Guild->GetGuildId();
    }

    // Создаём SaveGame объект
    UPlayerSaveGame* SaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass()));
    if (!SaveGame) return;

    SaveGame->PlayerData = SaveData;

    FString SlotName = GetPlayerSaveSlot(Player->GetUniqueId());
    UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
}

bool UPlayerSaveManager::LoadPlayer(APlayerState* Player)
{
    if (!IsServer() || !Player) return false;

    FString SlotName = GetPlayerSaveSlot(Player->GetUniqueId());
    if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0)) return false;

    UPlayerSaveGame* LoadGame = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if (!LoadGame) return false;

    FPlayerSaveData& SaveData = LoadGame->PlayerData;

    APawn* Pawn = Player->GetPawn();
    if (!Pawn) return false; // не можем применить без персонажа

    // Progression
    if (UPlayerProgressionComponent* Prog = Pawn->FindComponentByClass<UPlayerProgressionComponent>())
    {
        // Prog->SetWeaponMastery(SaveData.WeaponMastery);
    }

    // Economy
    if (UPlayerEconomyComponent* Econ = Pawn->FindComponentByClass<UPlayerEconomyComponent>())
    {
        Econ->Gold = SaveData.Gold;
        // Восстановить ресурсы и предметы
    }

    // Mounts
    if (UPlayerMountComponent* Mounts = Pawn->FindComponentByClass<UPlayerMountComponent>())
    {
        // Mounts->SetOwnedMounts(SaveData.OwnedMountIDs);
    }

    // Guild
    if (UGuildComponent* Guild = Pawn->FindComponentByClass<UGuildComponent>())
    {
        // Восстанавливаем принадлежность к гильдии (ранг нужно хранить отдельно)
        Guild->SetGuild(SaveData.GuildId, EGuildRank::Recruit); // ранг нужно будет потом обновить
    }

    return true;
}

void UPlayerSaveManager::SaveAllPlayers()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->PlayerState)
        {
            SavePlayer(PC->PlayerState);
        }
    }
}
