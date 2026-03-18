#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCPerceptionTypes.h"
#include "NPCPerceptionComponent.generated.h"

UCLASS(ClassGroup=(NPC), meta=(BlueprintSpawnableComponent))
class NPCPERCEPTIONCORE_API UNPCPerceptionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNPCPerceptionComponent();

    virtual void BeginPlay() override;

    void RegisterPerceptionEvent(const FNPCPerceptionEvent& Event);

    const TArray<FNPCPerceptionEvent>& GetRecentEvents() const { return RecentEvents; }

    UPROPERTY(EditAnywhere, Category = "Perception")
    float SightRadius = 2000.0f;

    UPROPERTY(EditAnywhere, Category = "Perception")
    float HearingRadius = 1200.0f;

protected:
    UPROPERTY()
    TArray<FNPCPerceptionEvent> RecentEvents;

    static constexpr int32 MAX_RECENT_EVENTS = 32;
};
