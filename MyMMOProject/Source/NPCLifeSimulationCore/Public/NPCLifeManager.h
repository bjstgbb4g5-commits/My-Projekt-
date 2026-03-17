#pragma once

#include "CoreMinimal.h"
#include "NPCLifeTypes.h"
#include "NPCLifeManager.generated.h"

class UNPCLifeComponent;

UCLASS()
class NPCLIFESIMULATIONCORE_API UNPCLifeManager : public UObject
{
    GENERATED_BODY()

public:
    void Initialize();
    void Shutdown();

    void RegisterNPC(AActor* NPC, const FNPCLifeData& Data);
    void UnregisterNPC(AActor* NPC);

    void UpdateSimulation(float DeltaTime, float CurrentHour);

    int32 GetNPCCount() const { return NPCComponents.Num(); }

    void SetGlobalLODBias(int32 Bias) { LODBias = Bias; }

private:
    TArray<UNPCLifeComponent*> NPCComponents;
    TMap<AActor*, FNPCLifeData> NPCDatas;
    int32 LODBias = 0;

    int32 CalculateLOD(AActor* NPCActor) const;
};
