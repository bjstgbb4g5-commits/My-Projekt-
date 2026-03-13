#include "InterestManagementSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "WorldGeneratorCore/Public/WorldGeneratorSubsystem.h"
#include "WorldPopulationCore/Public/WorldPopulationSubsystem.h"

UInterestManagementSubsystem::UInterestManagementSubsystem()
{
}

void UInterestManagementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UInterestManagementSubsystem::Deinitialize()
{
    PlayerInterestMap.Empty();
    Super::Deinitialize();
}

bool UInterestManagementSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

void UInterestManagementSubsystem::UpdatePlayerInterest(APlayerController* Player)
{
    if (!IsServer() || !Player) return;
    APawn* Pawn = Player->GetPawn();
    if (!Pawn) return;

    FVector PlayerLocation = Pawn->GetActorLocation();

    // Получаем все акторы (можно через ActorRegistry, но пока упростим)
    UWorld* World = GetWorld();
    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsWithTag(World, FName("Relevant"), AllActors);

    TArray<FActorInterestData> RelevantActors;
    for (AActor* Actor : AllActors)
    {
        if (!Actor) continue;
        float Dist = FVector::Dist(PlayerLocation, Actor->GetActorLocation());
        float MaxDist = 0.f;
        if (Actor->IsA<APlayerController>()) MaxDist = PlayerDistance;
        else if (Actor->IsA<ACharacter>() && Actor->ActorHasTag(FName("Monster"))) MaxDist = MonsterDistance;
        else if (Actor->IsA<AActor>() && Actor->ActorHasTag(FName("Resource"))) MaxDist = ResourceDistance;
        else if (Actor->ActorHasTag(FName("Boss"))) MaxDist = BossDistance;
        else continue;

        if (Dist <= MaxDist)
        {
            FActorInterestData Data;
            Data.Actor = Actor;
            Data.DistanceToPlayer = Dist;
            RelevantActors.Add(Data);
        }
    }

    PlayerInterestMap.Add(Player, RelevantActors);
}

void UInterestManagementSubsystem::SetInterestDistances(float InPlayerDistance, float InMonsterDistance, float InResourceDistance, float InBossDistance)
{
    PlayerDistance = InPlayerDistance;
    MonsterDistance = InMonsterDistance;
    ResourceDistance = InResourceDistance;
    BossDistance = InBossDistance;
}
