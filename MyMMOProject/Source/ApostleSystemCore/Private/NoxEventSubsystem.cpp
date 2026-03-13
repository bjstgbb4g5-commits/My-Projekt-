#include "NoxEventSubsystem.h"
#include "NoxBossCharacter.h"
#include "NoxFragmentSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "NavigationSystem.h"

UNoxEventSubsystem::UNoxEventSubsystem()
    : NoxBoss(nullptr)
    , bNoxActive(false)
{
}

void UNoxEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        FragmentSubsystem = GetWorld()->GetSubsystem<UNoxFragmentSubsystem>();
    }
}

void UNoxEventSubsystem::Deinitialize()
{
    if (IsServer())
    {
        GetWorld()->GetTimerManager().ClearTimer(FragmentSpawnTimerHandle);
    }
    Super::Deinitialize();
}

bool UNoxEventSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UNoxEventSubsystem::IsTickable() const
{
    return IsServer() && bNoxActive;
}

TStatId UNoxEventSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UNoxEventSubsystem, STATGROUP_Tickables);
}

void UNoxEventSubsystem::Tick(float DeltaTime)
{
    // Можно добавить проверки состояния босса, обновление фазы и т.д.
}

void UNoxEventSubsystem::StartNoxEvent()
{
    if (!IsServer() || bNoxActive) return;

    UE_LOG(LogTemp, Log, TEXT("Nox event started!"));
    bNoxActive = true;
    SpawnNox();
}

void UNoxEventSubsystem::SpawnNox()
{
    UWorld* World = GetWorld();
    if (!World) return;

    // Определяем место спавна Нокса (например, в центре руин Аурелиона)
    FVector SpawnLocation = FVector(1000000.f, 1000000.f, 0.f); // пример

    FActorSpawnParameters SpawnParams;
    NoxBoss = World->SpawnActor<ANoxBossCharacter>(ANoxBossCharacter::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    if (NoxBoss)
    {
        NoxBoss->InitializeHealth(2000000.f); // очень много здоровья
        OnNoxSpawned.Broadcast();
    }
}

void UNoxEventSubsystem::CompleteNoxEvent()
{
    if (!IsServer() || !bNoxActive) return;

    UE_LOG(LogTemp, Log, TEXT("Nox defeated!"));
    bNoxActive = false;
    NoxBoss = nullptr;

    // Генерируем осколки Нокса
    if (FragmentSubsystem)
    {
        FragmentSubsystem->GenerateFragments(24);
    }

    // Запускаем событие для игроков
    OnNoxDefeated.Broadcast();
}
