#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EconomyTypes.h"
#include "CurrencySubsystem.generated.h"

UCLASS()
class WORLDECONOMYCORE_API UCurrencySubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UCurrencySubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Economy|Currency")
    float ConvertCurrency(ECurrencyType FromType, ECurrencyType ToType, float Amount, int32 CityID = -1);

    UFUNCTION(BlueprintCallable, Category = "Economy|Currency")
    void UpdateExchangeRates();

private:
    bool IsServer() const;
    void ScheduleUpdate(float IntervalSeconds);
    void OnUpdateTimer();

    FTimerHandle UpdateTimerHandle;

    UPROPERTY()
    TArray<FCityCurrency> CityCurrencies;

    float GetGoldExchangeRate(ECurrencyType Type, int32 CityID) const;
};
