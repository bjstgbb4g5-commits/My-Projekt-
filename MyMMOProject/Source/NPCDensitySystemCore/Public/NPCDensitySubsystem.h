#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "Stats/Stats.h"
#include "NPCDensityManager.h"
#include "NPCDensitySubsystem.generated.h"

DECLARE_STATS_GROUP(TEXT("NPCDensity"), STATGROUP_NPCDensity, STATCAT_Advanced);

UCLASS()
class NPCDENSITYSYSTEMCORE_API UNPCDensitySubsystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    void RegisterZone(UNPCDensityZoneComponent* Zone);
    void UnregisterZone(UNPCDensityZoneComponent* Zone);

    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;
    virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
    virtual bool IsTickable() const override;

    UNPCDensityManager* GetManager() const { return Manager; }

private:
    UPROPERTY(Transient)
    UNPCDensityManager* Manager;

    float CurrentTime = 0.0f;
};
