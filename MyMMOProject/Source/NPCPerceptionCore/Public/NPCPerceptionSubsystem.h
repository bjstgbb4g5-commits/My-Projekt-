#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Stats/Stats.h"
#include "NPCPerceptionManager.h"
#include "NPCPerceptionSubsystem.generated.h"

DECLARE_STATS_GROUP(TEXT("NPCPerception"), STATGROUP_NPCPerception, STATCAT_Advanced);

UCLASS()
class NPCPERCEPTIONCORE_API UNPCPerceptionSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UNPCPerceptionManager* GetManager() const { return Manager; }

private:
    UPROPERTY(Transient)
    UNPCPerceptionManager* Manager;
};
