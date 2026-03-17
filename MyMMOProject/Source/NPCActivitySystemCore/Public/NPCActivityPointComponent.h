#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCActivityTypes.h"
#include "NPCActivityPointComponent.generated.h"

class UNPCActivityManager;

UCLASS(ClassGroup=(NPC), meta=(BlueprintSpawnableComponent))
class NPCACTIVITYSYSTEMCORE_API UNPCActivityPointComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNPCActivityPointComponent();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    bool HasFreeSlot(ENPCActivityType Activity) const;
    bool TryReserveSlot(AActor* NPC, ENPCActivityType Activity);
    bool ReleaseSlot(AActor* NPC);

    UPROPERTY(EditAnywhere, Category = "Activity")
    TArray<FActivitySlot> Slots;

    UPROPERTY(EditAnywhere, Category = "Activity")
    float InteractionRadius = 200.0f;

    int32 GetPointId() const { return PointId; }

private:
    friend class UNPCActivityManager;
    int32 PointId = -1;
};
