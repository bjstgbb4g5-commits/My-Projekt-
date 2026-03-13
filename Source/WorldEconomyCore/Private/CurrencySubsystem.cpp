#include "CurrencySubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UCurrencySubsystem::UCurrencySubsystem()
{
}

void UCurrencySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        // Загружаем начальные курсы (пример)
        FCityCurrency Gold;
        Gold.CityID = -1;
        Gold.CurrencyName = TEXT("Gold");
        Gold.ExchangeRateToGold = 1.0f;
        CityCurrencies.Add(Gold);

        ScheduleUpdate(300.0f); // каждые 5 минут
    }
}

void UCurrencySubsystem::Deinitialize()
{
    if (IsServer())
    {
        GetWorld()->GetTimerManager().ClearTimer(UpdateTimerHandle);
    }
    Super::Deinitialize();
}

bool UCurrencySubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

void UCurrencySubsystem::ScheduleUpdate(float IntervalSeconds)
{
    GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, this, &UCurrencySubsystem::OnUpdateTimer, IntervalSeconds, true);
}

void UCurrencySubsystem::OnUpdateTimer()
{
    UpdateExchangeRates();
}

float UCurrencySubsystem::GetGoldExchangeRate(ECurrencyType Type, int32 CityID) const
{
    if (Type == ECurrencyType::Gold) return 1.0f;

    if (Type == ECurrencyType::CityMark)
    {
        for (const FCityCurrency& CC : CityCurrencies)
        {
            if (CC.CityID == CityID)
                return CC.ExchangeRateToGold;
        }
    }
    return 0.0f;
}

float UCurrencySubsystem::ConvertCurrency(ECurrencyType FromType, ECurrencyType ToType, float Amount, int32 CityID)
{
    if (!IsServer()) return 0.0f;
    if (FromType == ToType) return Amount;

    float FromRate = GetGoldExchangeRate(FromType, CityID);
    float ToRate = GetGoldExchangeRate(ToType, CityID);

    if (FromRate <= 0.0f || ToRate <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid currency conversion rates: FromRate=%f, ToRate=%f"), FromRate, ToRate);
        return 0.0f;
    }

    float GoldAmount = Amount * FromRate;
    return GoldAmount / ToRate;
}

void UCurrencySubsystem::UpdateExchangeRates()
{
    // Случайные колебания курсов (заглушка)
    for (FCityCurrency& CC : CityCurrencies)
    {
        if (CC.CityID != -1)
        {
            float Delta = FMath::RandRange(-0.05f, 0.05f);
            CC.ExchangeRateToGold = FMath::Max(0.1f, CC.ExchangeRateToGold + Delta);
        }
    }
}
