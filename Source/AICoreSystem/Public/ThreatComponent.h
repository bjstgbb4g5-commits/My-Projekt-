#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AITypes.h"
#include "ThreatComponent.generated.h"

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AICORESYSTEM_API UThreatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UThreatComponent();

    UFUNCTION(BlueprintCallable, Category = "Threat")
    void AddThreat(AActor* Target, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Threat")
    void RemoveThreat(AActor* Target);

    UFUNCTION(BlueprintCallable, Category = "Threat")
    AActor* GetTopThreat() const;

    UFUNCTION(BlueprintCallable, Category = "Threat")
    TArray<AActor*> GetTargetsInOrder() const;

    UFUNCTION(BlueprintCallable, Category = "Threat")
    void ResetThreat();

private:
    UPROPERTY()
    TArray<FThreatEntry> ThreatList;

    void SortThreatList();
};
