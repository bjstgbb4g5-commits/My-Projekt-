#pragma once

#include "CoreMinimal.h"
#include "NPCActivityTypes.h"
#include "NPCActivityPointComponent.h"
#include "NPCActivityManager.generated.h"

UCLASS()
class NPCACTIVITYSYSTEMCORE_API UNPCActivityManager : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UWorld* InWorld);
    void Shutdown();

    void RegisterActivityPoint(UNPCActivityPointComponent* Point);
    void UnregisterActivityPoint(UNPCActivityPointComponent* Point);

    UNPCActivityPointComponent* FindActivityPoint(ENPCActivityType Activity, const FVector& NPCLocation, float MaxDistance = 2000.0f);

    bool RequestActivity(AActor* NPC, ENPCActivityType Activity, const FVector& NPCLocation, UNPCActivityPointComponent*& OutPoint);

    void Update(float DeltaTime);

private:
    UPROPERTY()
    UWorld* World;

    UPROPERTY()
    TArray<UNPCActivityPointComponent*> ActivityPoints;

    float UpdateTimer = 0.0f;
    static constexpr float UPDATE_INTERVAL = 5.0f;
};
