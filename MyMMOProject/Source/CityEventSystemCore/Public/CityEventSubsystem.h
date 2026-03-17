#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "Stats/Stats.h"
#include "CityEventManager.h"
#include "CityEventSubsystem.generated.h"

DECLARE_STATS_GROUP(TEXT("CityEvent"), STATGROUP_CityEvent, STATCAT_Advanced);

UCLASS()
class CITYEVENTSYSTEMCORE_API UCityEventSubsystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;
    virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
    virtual bool IsTickable() const override;

private:
    UPROPERTY(Transient)
    UCityEventManager* Manager;

    float CurrentTime = 0.0f;
};
