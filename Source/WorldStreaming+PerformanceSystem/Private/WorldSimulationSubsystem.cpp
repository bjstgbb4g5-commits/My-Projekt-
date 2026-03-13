#include "WorldSimulationSubsystem.h"
#include "Engine/World.h"
#include "Containers/Ticker.h"
#include "InterestManagementSubsystem.h"
#include "AILODComponent.h"
#include "AsyncSpawnSubsystem.h"
#include "ActorRegistrySubsystem.h" // предполагается, что есть
#include "CharacterEntity.h"

UWorldSimulationSubsystem::UWorldSimulationSubsystem()
{
}

void UWorldSimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UWorldSimulationSubsystem::Tick), 0.5f);
    }
}

void UWorldSimulationSubsystem::Deinitialize()
{
    FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
    Super::Deinitialize();
}

bool UWorldSimulationSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UWorldSimulationSubsystem::Tick(float DeltaTime)
{
    double StartTime = FPlatformTime::Seconds();

    // Обновление Interest Management для всех игроков
    UInterestManagementSubsystem* Interest = GetWorld()->GetSubsystem<UInterestManagementSubsystem>();
    if (Interest)
    {
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* PC = It->Get();
            if (PC) Interest->UpdatePlayerInterest(PC);
        }
    }

    // Обновление AI LOD
    UActorRegistrySubsystem* Registry = GetWorld()->GetSubsystem<UActorRegistrySubsystem>();
    if (Registry)
    {
        const TSet<AActor*>& AIActors = Registry->GetActorsByType(EActorType::AI);
        Metrics.ActiveAI = AIActors.Num();
        for (AActor* Actor : AIActors)
        {
            if (!Actor) continue;
            UAILODComponent* LODComp = Actor->FindComponentByClass<UAILODComponent>();
            if (LODComp)
            {
                LODComp->UpdateLOD(FVector::Dist(Actor->GetActorLocation(), FVector::ZeroVector)); // заглушка
            }
        }
    }

    // Обработка очереди спавна
    UAsyncSpawnSubsystem* Spawner = GetWorld()->GetSubsystem<UAsyncSpawnSubsystem>();
    if (Spawner) Spawner->ProcessSpawnQueue();

    UpdateAggro();
    UpdateStamina();

    Metrics.ServerTickTime = (FPlatformTime::Seconds() - StartTime) * 1000.f;
    Metrics.ActivePlayers = GetWorld()->GetNumPlayerControllers();

    return true;
}

void UWorldSimulationSubsystem::UpdateAggro()
{
    // Заглушка
}

void UWorldSimulationSubsystem::UpdateStamina()
{
    // Заглушка
}
