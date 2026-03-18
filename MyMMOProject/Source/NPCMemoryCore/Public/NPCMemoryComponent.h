
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCMemoryTypes.h"
#include "NPCMemoryComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NPCMEMORYCORE_API UNPCMemoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNPCMemoryComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Memory")
	int32 MaxMemoryRecords = 50;

	UPROPERTY(EditDefaultsOnly, Category = "Memory")
	float MemoryLifetime = 300.0f;

	UFUNCTION(BlueprintCallable, Category = "Memory")
	void AddMemory(const FNPCMemoryRecord& NewMemory);

	UFUNCTION(BlueprintCallable, Category = "Memory")
	const TArray<FNPCMemoryRecord>& GetMemories() const { return Memories; }

	UFUNCTION(BlueprintCallable, Category = "Memory")
	void ForgetOldMemories(float MaxAge);

	UFUNCTION(BlueprintCallable, Category = "Memory")
	void LimitMemory(int32 MaxRecords);

	UFUNCTION(BlueprintCallable, Category = "Memory")
	void ClearMemory();

	void UpdateMemory(float CurrentTime);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Memory")
	TArray<FNPCMemoryRecord> Memories;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	bool IsDuplicate(const FNPCMemoryRecord& NewMemory) const;
};
