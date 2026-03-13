#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PerformanceTypes.h"
#include "InterestManagementSubsystem.generated.h"

USTRUCT()
struct FActorInterestData
{
    GENERATED_BODY()

    AActor* Actor;
    float DistanceToPlayer;
};

UCLASS()
class WORLDPERFORMANCECORE_API UInterestManagementSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UInterestManagementSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    void UpdatePlayerInterest(APlayerController* Player);

    void SetInterestDistances(float InPlayerDistance, float InMonsterDistance, float InResourceDistance, float InBossDistance);

private:
    bool IsServer() const;

    UPROPERTY()
    TMap<APlayerController*, TArray<FActorInterestData>> PlayerInterestMap;

    float PlayerDistance = 20000.f;
    float MonsterDistance = 12000.f;
    float ResourceDistance = 8000.f;
    float BossDistance = 30000.f;
};
