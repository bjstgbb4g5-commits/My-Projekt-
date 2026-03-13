#include "BanManager.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void UBanManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    LoadBanList();
}

void UBanManager::Deinitialize()
{
    SaveBanList();
    Super::Deinitialize();
}

void UBanManager::LoadBanList()
{
    if (!UGameplayStatics::DoesSaveGameExist(TEXT("BanList"), 0))
    {
        SaveGame = Cast<UBanSaveGame>(UGameplayStatics::CreateSaveGameObject(UBanSaveGame::StaticClass()));
        SaveBanList();
    }
    else
    {
        SaveGame = Cast<UBanSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("BanList"), 0));
    }

    if (!SaveGame)
    {
        SaveGame = Cast<UBanSaveGame>(UGameplayStatics::CreateSaveGameObject(UBanSaveGame::StaticClass()));
        SaveBanList();
    }
}

void UBanManager::SaveBanList()
{
    UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("BanList"), 0);
}

bool UBanManager::IsPlayerBanned(const FUniqueNetIdRepl& PlayerID) const
{
    FString IDStr = PlayerID.ToString();
    for (const FBanRecord& Record : SaveGame->BannedPlayers)
    {
        if (Record.PlayerID == IDStr)
            return true;
    }
    return false;
}

void UBanManager::BanPlayer(const FUniqueNetIdRepl& PlayerID, const FString& Reason)
{
    if (IsPlayerBanned(PlayerID)) return;

    FBanRecord NewBan;
    NewBan.PlayerID = PlayerID.ToString();
    NewBan.BanTime = FDateTime::UtcNow();
    NewBan.Reason = Reason;
    SaveGame->BannedPlayers.Add(NewBan);
    SaveBanList();

    KickPlayer(PlayerID, Reason);
}

void UBanManager::KickPlayer(const FUniqueNetIdRepl& PlayerID, const FString& Reason)
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->PlayerState && PC->PlayerState->GetUniqueId() == PlayerID)
        {
            PC->ClientReturnToMainHost(FString::Printf(TEXT("You have been kicked: %s"), *Reason));
            break;
        }
    }
}
