
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "SocialManager.h"
#include "SocialSubsystem.generated.h"

UCLASS()
class SOCIALSIMULATIONCORE_API USocialSubsystem : public UWorldSubsystem, public FTickableGameObject
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

	UFUNCTION(BlueprintCallable, Category = "Social")
	USocialManager* GetSocialManager() const { return SocialManager; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Social")
	float UpdateInterval = 1.0f;

	float TimeSinceLastUpdate = 0.0f;

	UPROPERTY()
	USocialManager* SocialManager = nullptr;
};
