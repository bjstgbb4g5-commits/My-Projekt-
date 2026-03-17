#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCLifeTypes.h"
#include "NPCLifeComponent.generated.h"

UCLASS(ClassGroup=(NPC), meta=(BlueprintSpawnableComponent))
class NPCLIFESIMULATIONCORE_API UNPCLifeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNPCLifeComponent();

    virtual void BeginPlay() override;

    void InitializeFromData(const FNPCLifeData& InData);
    void UpdateLife(float CurrentHour);
    void PerformTask(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "NPC Life")
    void SetCurrentTask(ENPCDailyTask NewTask, AActor* TargetLocation = nullptr);

    UFUNCTION(BlueprintCallable, Category = "NPC Life")
    ENPCDailyTask GetCurrentTask() const { return CurrentTask; }

    UFUNCTION(BlueprintCallable, Category = "NPC Life")
    AActor* GetTargetLocation() const { return CurrentTarget; }

    void SetLODLevel(int32 InLOD) { LODLevel = InLOD; }
    int32 GetLODLevel() const { return LODLevel; }

private:
    FNPCLifeData LifeData;

    ENPCDailyTask CurrentTask = ENPCDailyTask::Idle;
    AActor* CurrentTarget = nullptr;
    float TaskStartTime = 0.0f;
    int32 LODLevel = 0;
};
