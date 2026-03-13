#include "WorldSaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "GuildSystemCore/Public/GuildSubsystem.h"
#include "WorldSaveGame.h"

void UWorldSaveManager::SaveWorld()
{
    if (!IsServer()) return;

    UWorldSaveGame* SaveGame = Cast<UWorldSaveGame>(UGameplayStatics::CreateSaveGameObject(UWorldSaveGame::StaticClass()));
    if (!SaveGame) return;

    // Сохраняем гильдии
    UGuildSubsystem* Guilds = GetWorld()->GetSubsystem<UGuildSubsystem>();
    if (Guilds)
    {
        Guilds->SaveGuilds(); // гильдии сохраняются отдельно, или можно интегрировать сюда
    }

    // Сохраняем территории, ресурсные узлы и т.д.
    // ...

    UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("World"), 0);
}

bool UWorldSaveManager::LoadWorld()
{
    if (!IsServer()) return false;

    if (!UGameplayStatics::DoesSaveGameExist(TEXT("World"), 0)) return false;

    UWorldSaveGame* LoadGame = Cast<UWorldSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("World"), 0));
    if (!LoadGame) return false;

    // Загружаем гильдии
    UGuildSubsystem* Guilds = GetWorld()->GetSubsystem<UGuildSubsystem>();
    if (Guilds)
    {
        Guilds->LoadGuilds();
    }

    // Загружаем остальное
    // ...

    return true;
}

bool UWorldSaveManager::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}
