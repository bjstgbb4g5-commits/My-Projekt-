#include "CityEconomyComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "CityPoliticalComponent.h"
#include "CityGovernanceComponent.h"

UCityEconomyComponent::UCityEconomyComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);

    CityBudget = 0;
    CityTaxPercent = 15.f;
}

void UCityEconomyComponent::BeginPlay()
{
    Super::BeginPlay();
    if (IsServer())
    {
        if (!LastEconomyCycle.IsValid())
        {
            LastEconomyCycle = GetUtcNow() - FTimespan::FromHours(1);
        }
    }
}

void UCityEconomyComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UCityEconomyComponent, CityBudget);
    DOREPLIFETIME(UCityEconomyComponent, CityTaxPercent);
    DOREPLIFETIME(UCityEconomyComponent, CityStorage);
    DOREPLIFETIME(UCityEconomyComponent, ActiveContracts);
    DOREPLIFETIME(UCityEconomyComponent, LastEconomyCycle);
}

bool UCityEconomyComponent::IsServer() const
{
    return GetOwner() && GetOwner()->HasAuthority();
}

FDateTime UCityEconomyComponent::GetUtcNow() const
{
    return FDateTime::UtcNow();
}

UCityPoliticalComponent* UCityEconomyComponent::GetCityPolitical() const
{
    return GetOwner() ? GetOwner()->FindComponentByClass<UCityPoliticalComponent>() : nullptr;
}

UCityGovernanceComponent* UCityEconomyComponent::GetCityGovernance() const
{
    return GetOwner() ? GetOwner()->FindComponentByClass<UCityGovernanceComponent>() : nullptr;
}

float UCityEconomyComponent::GetDoctrineProductionModifier(EResourceType Resource) const
{
    UCityGovernanceComponent* Gov = GetCityGovernance();
    if (!Gov) return 1.0f;

    switch (Gov->ActiveDoctrine)
    {
    case ECityDoctrine::Military:
        if (Resource == EResourceType::Ore) return 1.10f;
        break;
    case ECityDoctrine::Diplomatic:
        if (Resource == EResourceType::Fiber) return 1.10f;
        break;
    case ECityDoctrine::Progressive:
        if (Resource == EResourceType::Crystal) return 1.10f;
        break;
    default:
        break;
    }
    return 1.0f;
}

void UCityEconomyComponent::ProcessProduction()
{
    // Заглушка
}

void UCityEconomyComponent::CollectTax(int32 PlayerIncome)
{
    int32 TaxAmount = FMath::FloorToInt(PlayerIncome * (CityTaxPercent / 100.f));
    CityBudget += TaxAmount;

    if (CityTaxPercent > 20.f)
    {
        if (UCityPoliticalComponent* Pol = GetCityPolitical())
        {
            Pol->ModifyUnrest(5.f);
        }
        if (UCityGovernanceComponent* Gov = GetCityGovernance())
        {
            Gov->Legitimacy = FMath::Clamp(Gov->Legitimacy - 5.f, 0.f, 100.f);
        }
    }
}

void UCityEconomyComponent::ProcessEconomyCycle()
{
    if (!IsServer()) return;
    FDateTime Now = GetUtcNow();
    if (Now < LastEconomyCycle + FTimespan::FromHours(24)) return;

    ProcessProduction();
    DistributeGuildBudget();
    UpdateDeficitState();
    LastEconomyCycle = Now;
}

void UCityEconomyComponent::DistributeGuildBudget()
{
    // Заглушка
}

int32 UCityEconomyComponent::CreateContract(EContractType Type, int32 TargetAmount, int32 RewardGold)
{
    if (!IsServer()) return -1;
    if (CityBudget < RewardGold) return -1;

    CityBudget -= RewardGold;
    FMarketOrder NewOrder;
    NewOrder.OrderID = ActiveContracts.Num() + 1;
    NewOrder.Price = RewardGold;
    ActiveContracts.Add(NewOrder);
    return NewOrder.OrderID;
}

bool UCityEconomyComponent::CompleteContract(int32 ContractId, APlayerState* Player)
{
    // Заглушка
    return false;
}

void UCityEconomyComponent::UpdateDeficitState()
{
    // Заглушка
}

int32 UCityEconomyComponent::GetTotalConsumption(EResourceType Type) const
{
    return 0; // заглушка
}
