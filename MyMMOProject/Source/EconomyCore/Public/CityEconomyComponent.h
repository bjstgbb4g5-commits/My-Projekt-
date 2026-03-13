#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EconomyTypes.h"
#include "CityEconomyComponent.generated.h"

class UCityPoliticalComponent;
class UCityGovernanceComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ECONOMYCORE_API UCityEconomyComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCityEconomyComponent();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Economy")
    int32 CityBudget;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Economy")
    float CityTaxPercent;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Economy")
    TMap<EResourceType, int32> CityStorage;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Economy")
    TArray<FMarketOrder> ActiveContracts;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Economy")
    FDateTime LastEconomyCycle;

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void ProcessProduction();

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void CollectTax(int32 PlayerIncome);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void ProcessEconomyCycle();

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void DistributeGuildBudget();

    UFUNCTION(BlueprintCallable, Category = "Economy")
    int32 CreateContract(EContractType Type, int32 TargetAmount, int32 RewardGold);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    bool CompleteContract(int32 ContractId, APlayerState* Player);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void UpdateDeficitState();

private:
    bool IsServer() const;
    FDateTime GetUtcNow() const;
    UCityPoliticalComponent* GetCityPolitical() const;
    UCityGovernanceComponent* GetCityGovernance() const;
    float GetDoctrineProductionModifier(EResourceType Resource) const;
    int32 GetTotalConsumption(EResourceType Type) const;
};
