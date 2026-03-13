#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EconomyTypes.h"
#include "PlayerEconomyComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ECONOMYCORE_API UPlayerEconomyComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPlayerEconomyComponent();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Economy")
    int32 Gold;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Economy")
    TMap<EResourceType, int32> Resources;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Economy")
    TArray<FItemStack> Items;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Economy")
    int32 TradeReputation;

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void AddGold(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    bool RemoveGold(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    bool HasGold(int32 Amount) const { return Gold >= Amount; }

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void AddResource(EResourceType Type, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    bool RemoveResource(EResourceType Type, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    int32 GetResourceAmount(EResourceType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void AddItem(int32 ItemID, int32 Quantity, bool bStolen);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    bool RemoveItem(int32 ItemID, int32 Quantity);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    bool HasItem(int32 ItemID, int32 Quantity) const;

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void NotifyIncome(int32 Amount);

private:
    bool IsServer() const;
    UCityEconomyComponent* GetCityEconomy() const;
};
