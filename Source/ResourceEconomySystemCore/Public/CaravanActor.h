#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaravanTypes.h"
#include "CaravanActor.generated.h"

UCLASS()
class CARAVANSYSTEMCORE_API ACaravanActor : public AActor
{
    GENERATED_BODY()

public:
    ACaravanActor();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Caravan")
    void InitializeCaravan(int32 CaravanID, const FCaravanData& Data);

    UFUNCTION(BlueprintCallable, Category = "Caravan")
    void SetUnderAttack(bool bAttacked);

    UFUNCTION(BlueprintCallable, Category = "Caravan")
    void UpdateLocation(const FVector& NewLocation);

    UPROPERTY(ReplicatedUsing = OnRep_CaravanData)
    FCaravanData CaravanData;

    UFUNCTION()
    void OnRep_CaravanData();

    // Событие для уведомления PvPWarfareCore об атаке
    DECLARE_EVENT_OneParam(ACaravanActor, FOnCaravanAttacked, int32)
    FOnCaravanAttacked OnCaravanAttacked;

protected:
    UPROPERTY()
    class UCaravanSubsystem* CaravanSubsystem;

    UPROPERTY()
    class UWorldTerritorySystemCore* TerritorySystem;

    void UpdateTerritoryAndRisk();

    FTimerHandle AttackTimerHandle;
};
