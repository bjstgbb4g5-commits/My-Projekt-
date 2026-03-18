#pragma once

#include "CoreMinimal.h"
#include "NPCPerceptionTypes.h"
#include "NPCPerceptionManager.generated.h"

UCLASS()
class NPCPERCEPTIONCORE_API UNPCPerceptionManager : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UWorld* InWorld);
    void Shutdown();

    void RegisterNPC(AActor* NPC);
    void UnregisterNPC(AActor* NPC);

    void BroadcastEvent(const FNPCPerceptionEvent& Event);

    int32 GetRegisteredNPCCount() const { return RegisteredNPCs.Num(); }

private:
    UPROPERTY()
    UWorld* World;

    UPROPERTY()
    TArray<AActor*> RegisteredNPCs;
};
