#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PvPTypes.h"
#include "SiegeMachine.generated.h"

UCLASS()
class PVPWARFARECORE_API ASiegeMachine : public APawn
{
    GENERATED_BODY()

public:
    ASiegeMachine();

    UFUNCTION(BlueprintCallable, Category = "Siege")
    void Fire();

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Siege")
    void Server_Fire();

    UFUNCTION(BlueprintCallable, Category = "Siege")
    void TakeDamage(float Damage, AController* Instigator);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnDestroyed();

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Siege")
    FSiegeMachineData Data;

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCapsuleComponent* Collision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

private:
    UPROPERTY(Replicated)
    float CurrentHealth;

    UPROPERTY(Replicated)
    float CurrentReload;

    void OnDeath();
};
