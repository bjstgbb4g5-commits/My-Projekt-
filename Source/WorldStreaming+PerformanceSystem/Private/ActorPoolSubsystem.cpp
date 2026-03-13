#include "ActorPoolSubsystem.h"
#include "Engine/World.h"

UActorPoolSubsystem::UActorPoolSubsystem()
{
}

void UActorPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UActorPoolSubsystem::Deinitialize()
{
    ActorPools.Empty();
    Super::Deinitialize();
}

AActor* UActorPoolSubsystem::AcquireActor(UClass* ActorClass, const FTransform& Transform)
{
    if (!ActorClass) return nullptr;

    FActorPool& Pool = ActorPools.FindOrAdd(ActorClass);
    for (int32 i = Pool.AvailableActors.Num() - 1; i >= 0; --i)
    {
        AActor* Actor = Pool.AvailableActors[i];
        if (IsValid(Actor))
        {
            Pool.AvailableActors.RemoveAt(i);
            Actor->SetActorTransform(Transform);
            Actor->SetActorHiddenInGame(false);
            Actor->SetActorEnableCollision(true);
            Actor->SetActorTickEnabled(true);
            Pool.ActiveActors.Add(Actor);
            return Actor;
        }
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass, Transform, SpawnParams);
    if (NewActor)
    {
        Pool.ActiveActors.Add(NewActor);
    }
    return NewActor;
}

void UActorPoolSubsystem::ReleaseActor(AActor* Actor)
{
    if (!Actor) return;
    UClass* ActorClass = Actor->GetClass();
    FActorPool& Pool = ActorPools.FindOrAdd(ActorClass);
    Pool.ActiveActors.Remove(Actor);
    if (Pool.AvailableActors.Num() < MaxPoolSize)
    {
        Actor->SetActorHiddenInGame(true);
        Actor->SetActorEnableCollision(false);
        Actor->SetActorTickEnabled(false);
        Pool.AvailableActors.Add(Actor);
    }
    else
    {
        Actor->Destroy();
    }
}

void UActorPoolSubsystem::PrePoolActors(UClass* ActorClass, int32 Count)
{
    if (!ActorClass) return;
    FActorPool& Pool = ActorPools.FindOrAdd(ActorClass);
    int32 ToSpawn = Count - Pool.AvailableActors.Num();
    for (int32 i = 0; i < ToSpawn; ++i)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass, FTransform::Identity, SpawnParams);
        if (NewActor)
        {
            NewActor->SetActorHiddenInGame(true);
            NewActor->SetActorEnableCollision(false);
            NewActor->SetActorTickEnabled(false);
            Pool.AvailableActors.Add(NewActor);
        }
    }
}
