#pragma once

#include "CoreMinimal.h"
#include "NPCLifeTypes.h"
#include "NPCLifeScheduler.generated.h"

UCLASS()
class NPCLIFESIMULATIONCORE_API UNPCLifeScheduler : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(AActor* InNPC, const FNPCLifeData& InData);
    void Update(float CurrentHour);

private:
    TWeakObjectPtr<AActor> NPC;
    FNPCLifeData LifeData;
    int32 CurrentScheduleIndex = -1;
};
