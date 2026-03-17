#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "Stats/Stats.h"
#include "NPCActivityManager.h"
#include "NPCActivitySubsystem.generated.h"

DECLARE_STATS_GROUP(TEXT("NPCActivity"), STATGROUP_NPCActivity, STATCAT_Advanced);

UCLASS()
class NPCACTIVITYSYSTEMCORE_API UNPCActivitySubsystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;
    virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
    virtual bool IsTickable() const override;

    void RegisterActivityPoint(UNPCActivityPointComponent* Point);
    void UnregisterActivityPoint(UNPCActivityPointComponent* Point);

    UNPCActivityManager* GetManager() const { return Manager; }

private:
    UPROPERTY(Transient)
    UNPCActivityManager* Manager;

    float CurrentTime = 0.0f;
};
