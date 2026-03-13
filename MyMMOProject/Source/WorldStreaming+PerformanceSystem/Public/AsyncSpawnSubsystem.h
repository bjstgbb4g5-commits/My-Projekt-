#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AsyncSpawnSubsystem.generated.h"

USTRUCT()
struct FSpawnRequest
{
    GENERATED_BODY()

    UClass* ActorClass = nullptr;
    FVector Location;
    FRotator Rotation;
};

UCLASS()
class WORLDPERFORMANCECORE_API UAsyncSpawnSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UAsyncSpawnSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Async Spawn")
    void RequestSpawn(UClass* ActorClass, const FVector& Location, const FRotator& Rotation = FRotator::ZeroRotator);

    void ProcessSpawnQueue();

private:
    bool IsServer() const;
    int32 CalculateSpawnLimit() const;

    TQueue<FSpawnRequest> SpawnQueue;
    int32 SpawnsPerTick = 20;
    TFunction<float()> GetTickTime;
};
