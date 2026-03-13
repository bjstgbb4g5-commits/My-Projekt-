#include "PlayerRegistrySubsystem.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void UPlayerRegistrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UPlayerRegistrySubsystem::Deinitialize()
{
    PlayerMap.Empty();
    Super::Deinitialize();
}

void UPlayerRegistrySubsystem::RegisterPlayer(APlayerController* Player)
{
    if (!Player) return;
    FUniqueNetIdRepl Id = Player->PlayerState ? Player->PlayerState->GetUniqueId() : FUniqueNetIdRepl();
    if (Id.IsValid())
    {
        PlayerMap.Add(Id, Player);
    }
}

void UPlayerRegistrySubsystem::UnregisterPlayer(APlayerController* Player)
{
    if (!Player) return;
    FUniqueNetIdRepl Id = Player->PlayerState ? Player->PlayerState->GetUniqueId() : FUniqueNetIdRepl();
    if (Id.IsValid())
    {
        PlayerMap.Remove(Id);
    }
}

APlayerController* UPlayerRegistrySubsystem::GetPlayerById(const FUniqueNetIdRepl& PlayerId) const
{
    return PlayerMap.FindRef(PlayerId);
}

TArray<APlayerController*> UPlayerRegistrySubsystem::GetPlayersInRegion(int32 RegionIndex) const
{
    // Заглушка: нужна интеграция с WorldGeneratorCore для получения региона по координатам
    return TArray<APlayerController*>();
}

TArray<APlayerController*> UPlayerRegistrySubsystem::GetAllPlayers() const
{
    TArray<APlayerController*> Result;
    PlayerMap.GenerateValueArray(Result);
    return Result;
}
