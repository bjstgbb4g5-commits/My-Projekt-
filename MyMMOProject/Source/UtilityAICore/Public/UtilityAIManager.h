
#pragma once

#include "CoreMinimal.h"
#include "UtilityAIManager.generated.h"

class UUtilityAIComponent;

UCLASS()
class UTILITYAICORE_API UUtilityAIManager : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UWorld* InWorld);
    void Shutdown();

    void RegisterNPC(AActor* NPC);
    void UnregisterNPC(AActor* NPC);

    void UpdateAllAI();

    int32 GetRegisteredNPCCount() const { return NPCList.Num(); }

private:
    UPROPERTY()
    UWorld* World;

    TArray<TWeakObjectPtr<AActor>> NPCList;
};
