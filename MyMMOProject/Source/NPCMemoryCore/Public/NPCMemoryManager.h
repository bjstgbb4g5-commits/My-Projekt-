
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NPCMemoryTypes.h"
#include "NPCMemoryComponent.h"
#include "NPCMemoryManager.generated.h"

UCLASS()
class NPCMEMORYCORE_API UNPCMemoryManager : public UObject
{
	GENERATED_BODY()

public:
	void RegisterComponent(UNPCMemoryComponent* MemoryComp);
	void UnregisterComponent(UNPCMemoryComponent* MemoryComp);

	UFUNCTION(BlueprintCallable, Category = "Memory")
	void BroadcastEvent(const FNPCMemoryRecord& Event, float Radius = 0.0f, const FVector& Origin = FVector::ZeroVector);

	void UpdateAllMemories(float CurrentTime);

private:
	TArray<TWeakObjectPtr<UNPCMemoryComponent>> RegisteredComponents;
};
