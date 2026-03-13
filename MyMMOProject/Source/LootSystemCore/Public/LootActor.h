#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryTypes.h"
#include "LootActor.generated.h"

class UBoxComponent;

UCLASS()
class LOOTSYSTEMCORE_API ALootActor : public AActor
{
    GENERATED_BODY()

public:
    ALootActor();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Предмет, который содержит этот лут
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Loot")
    FItemInstance Item;

    // Радиус взаимодействия (триггер)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Loot")
    UBoxComponent* InteractionTrigger;

    // Время жизни (сек) – уничтожится, если никто не подобрал
    UPROPERTY(EditDefaultsOnly, Category = "Loot")
    float LifespanSeconds = 300.0f; // 5 минут

    // Вызывается при попытке подобрать лут (через RPC)
    UFUNCTION(BlueprintCallable, Category = "Loot")
    void Pickup(APlayerController* Player);

protected:
    UFUNCTION(Server, Reliable)
    void Server_Pickup(APlayerController* Player);

    virtual void OnPickedUp(APlayerController* Player);

    FTimerHandle LifespanTimerHandle;
    void OnLifespanExpired();

private:
    float LastPickupTime = -10.f; // для анти‑спама
};
