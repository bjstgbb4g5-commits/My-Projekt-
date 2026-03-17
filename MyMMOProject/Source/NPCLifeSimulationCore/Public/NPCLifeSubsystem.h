#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "NPCLifeManager.h"
#include "NPCLifeSubsystem.generated.h"

class UWorldSimulationSubsystem;

UCLASS()
class NPCLIFESIMULATIONCORE_API UNPCLifeSubsystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;
    virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
    virtual bool IsTickable() const override;

    UFUNCTION(BlueprintCallable, Category = "NPC Life")
    void RegisterNPC(AActor* NPC, const FNPCLifeData& Data);

    UFUNCTION(BlueprintCallable, Category = "NPC Life")
    void UnregisterNPC(AActor* NPC);

    UNPCLifeManager* GetManager() const { return Manager; }

private:
    UPROPERTY(Transient)
    UNPCLifeManager* Manager = nullptr;

    float CurrentHour = 0.0f;
    const float HOURS_PER_REAL_SECOND = 0.5f;

    UWorldSimulationSubsystem* WorldSim = nullptr;
};
