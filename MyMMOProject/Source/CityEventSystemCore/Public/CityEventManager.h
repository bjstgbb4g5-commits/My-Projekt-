#pragma once

#include "CoreMinimal.h"
#include "CityEventTypes.h"
#include "CityEventComponent.h"
#include "CityEventManager.generated.h"

class UNPCLifeSubsystem;
class UNPCDensitySubsystem;

UCLASS()
class CITYEVENTSYSTEMCORE_API UCityEventManager : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UWorld* InWorld);
    void Shutdown();

    void Update(float DeltaTime, float CurrentTime);
    bool SpawnRandomEvent(float CurrentTime);
    void EndEvent(int32 EventId, bool bForce = false);

    const TArray<FCityEvent>& GetActiveEvents() const { return ActiveEvents; }

private:
    UPROPERTY()
    UWorld* World;

    UPROPERTY()
    UNPCLifeSubsystem* NPCLifeSubsystem;

    UPROPERTY()
    UNPCDensitySubsystem* DensitySubsystem;

    TArray<FCityEvent> ActiveEvents;
    int32 NextEventId = 1;
    float EventSpawnTimer = 0.0f;
};
