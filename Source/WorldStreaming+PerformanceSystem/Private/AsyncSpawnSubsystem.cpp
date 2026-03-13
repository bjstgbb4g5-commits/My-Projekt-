#include "AsyncSpawnSubsystem.h"
#include "Engine/World.h"
#include "ActorPoolSubsystem.h"

UAsyncSpawnSubsystem::UAsyncSpawnSubsystem()
{
}

void UAsyncSpawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UAsyncSpawnSubsystem::Deinitialize()
{
    FSpawnRequest Dummy;
    while (SpawnQueue.Dequeue(Dummy)) {}
    Super::Deinitialize();
}

bool UAsyncSpawnSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

void UAsyncSpawnSubsystem::RequestSpawn(UClass* ActorClass, const FVector& Location, const FRotator& Rotation)
{
    FSpawnRequest Request;
    Request.ActorClass = ActorClass;
    Request.Location = Location;
    Request.Rotation = Rotation;
    SpawnQueue.Enqueue(Request);
}

int32 UAsyncSpawnSubsystem::CalculateSpawnLimit() const
{
    return SpawnsPerTick;
}

void UAsyncSpawnSubsystem::ProcessSpawnQueue()
{
    if (!IsServer()) return;
    UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
    if (!Pool) return;

    int32 Limit = CalculateSpawnLimit();
    int32 Spawned = 0;
    FSpawnRequest Request;
    while (Spawned < Limit && SpawnQueue.Dequeue(Request))
    {
        if (Request.ActorClass)
        {
            FTransform Transform(Request.Rotation, Request.Location);
            Pool->AcquireActor(Request.ActorClass, Transform);
        }
        Spawned++;
    }
}
