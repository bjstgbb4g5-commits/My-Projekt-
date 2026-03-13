#include "EcosystemSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UEcosystemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (GetWorld()->IsServer())
    {
        GetWorld()->GetTimerManager().SetTimer(EcosystemTimerHandle, [this]()
        {
            UpdateEcosystem(EcosystemTickRate);
        }, EcosystemTickRate, true);
    }
}

void UEcosystemSubsystem::Deinitialize()
{
    if (GetWorld()->IsServer())
    {
        GetWorld()->GetTimerManager().ClearTimer(EcosystemTimerHandle);
    }
    Super::Deinitialize();
}

void UEcosystemSubsystem::SpawnZonePopulation(int32 ZoneID)
{
    // Заглушка
}

void UEcosystemSubsystem::UpdateEcosystem(float DeltaTime)
{
    // Заглушка: обновление экосистемы
}
