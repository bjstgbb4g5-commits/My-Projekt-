
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "NPCMemoryManager.h"
#include "NPCMemorySubsystem.generated.h"

UCLASS()
class NPCMEMORYCORE_API UNPCMemorySubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override { return true; }
	virtual bool IsTickableWhenPaused() const override { return false; }
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }

	UFUNCTION(BlueprintCallable, Category = "Memory")
	UNPCMemoryManager* GetMemoryManager() const { return MemoryManager; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Memory")
	float UpdateInterval = 0.5f;

	float TimeSinceLastUpdate = 0.0f;

	UPROPERTY()
	UNPCMemoryManager* MemoryManager = nullptr;
};
