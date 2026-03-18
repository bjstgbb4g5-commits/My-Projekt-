
#pragma once

#include "CoreMinimal.h"
#include "CrowdTypes.h"
#include "CrowdAgentComponent.h"
#include "CrowdManager.generated.h"

UCLASS()
class CROWDSIMULATIONCORE_API UCrowdManager : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UWorld* InWorld);
    void Shutdown();

    void RegisterAgent(UCrowdAgentComponent* Agent);
    void UnregisterAgent(UCrowdAgentComponent* Agent);

    void UpdateCrowd(float DeltaTime);

    int32 GetAgentCount() const { return Agents.Num(); }

private:
    UPROPERTY()
    UWorld* World;

    TArray<TWeakObjectPtr<UCrowdAgentComponent>> Agents;
};
