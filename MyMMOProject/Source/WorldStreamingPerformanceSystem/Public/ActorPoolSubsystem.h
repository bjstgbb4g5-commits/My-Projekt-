#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActorPoolSubsystem.generated.h"

USTRUCT()
struct FActorPool
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<AActor*> AvailableActors;

    UPROPERTY()
    TArray<AActor*> ActiveActors;
};

UCLASS()
class WORLDPERFORMANCECORE_API UActorPoolSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UActorPoolSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Actor Pool")
    AActor* AcquireActor(UClass* ActorClass, const FTransform& Transform);

    UFUNCTION(BlueprintCallable, Category = "Actor Pool")
    void ReleaseActor(AActor* Actor);

    UFUNCTION(BlueprintCallable, Category = "Actor Pool")
    void PrePoolActors(UClass* ActorClass, int32 Count);

private:
    UPROPERTY()
    TMap<UClass*, FActorPool> ActorPools;

    int32 MaxPoolSize = 500;
};
