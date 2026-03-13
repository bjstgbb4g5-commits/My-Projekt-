#include "SaveScheduler.h"
#include "PlayerSaveManager.h"
#include "WorldSaveManager.h"
#include "Engine/World.h"

void USaveScheduler::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (GetWorld()->IsServer())
    {
        StartAutoSave(300); // каждые 5 минут
    }
}

void USaveScheduler::Deinitialize()
{
    StopAutoSave();
    Super::Deinitialize();
}

void USaveScheduler::StartAutoSave(int32 IntervalSeconds)
{
    GetWorld()->GetTimerManager().SetTimer(AutoSaveTimerHandle, this, &USaveScheduler::AutoSave, IntervalSeconds, true);
}

void USaveScheduler::StopAutoSave()
{
    GetWorld()->GetTimerManager().ClearTimer(AutoSaveTimerHandle);
}

void USaveScheduler::AutoSave()
{
    if (bSaveInProgress) return;
    bSaveInProgress = true;

    UPlayerSaveManager* PlayerSave = GetWorld()->GetSubsystem<UPlayerSaveManager>();
    if (PlayerSave) PlayerSave->SaveAllPlayers();

    UWorldSaveManager* WorldSave = GetWorld()->GetSubsystem<UWorldSaveManager>();
    if (WorldSave) WorldSave->SaveWorld();

    bSaveInProgress = false;
}
