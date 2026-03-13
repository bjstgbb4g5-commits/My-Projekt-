#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PvETypes.h"
#include "WorldEcosystemSubsystem.generated.h"

UCLASS()
class PVEWORLDCORE_API UWorldEcosystemSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UWorldEcosystemSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void SpawnZonePopulation(int32 ZoneID);

    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void DespawnFarCreatures();

    void UpdateEcosystem();

private:
    bool IsServer() const;
    void ScheduleUpdate(float IntervalSeconds);
    void OnUpdateTimer();

    FTimerHandle UpdateTimerHandle;

    UPROPERTY()
    TArray<FEcosystemZone> Zones;

    static constexpr float ECOSYSTEM_UPDATE_INTERVAL = 5.f;
};
