#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EcosystemTypes.h"
#include "EcosystemSubsystem.generated.h"

UCLASS()
class WORLDECOSYSTEMCORE_API UEcosystemSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void SpawnZonePopulation(int32 ZoneID);

    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void UpdateEcosystem(float DeltaTime);

protected:
    UPROPERTY()
    TArray<FEcosystemZone> Zones;

    float EcosystemTickRate = 5.f;
    FTimerHandle EcosystemTimerHandle;
};
