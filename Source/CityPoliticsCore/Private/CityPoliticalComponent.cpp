#include "CityPoliticalComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"

UCityPoliticalComponent::UCityPoliticalComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);

    Prosperity = 100.f;
    Loyalty = 100.f;
    Unrest = 0.f;
}

void UCityPoliticalComponent::BeginPlay()
{
    Super::BeginPlay();
    ClampValues();
}

void UCityPoliticalComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UCityPoliticalComponent, Prosperity);
    DOREPLIFETIME(UCityPoliticalComponent, Loyalty);
    DOREPLIFETIME(UCityPoliticalComponent, Unrest);
    DOREPLIFETIME(UCityPoliticalComponent, Districts);
}

void UCityPoliticalComponent::ClampValues()
{
    Prosperity = FMath::Clamp(Prosperity, 0.f, 100.f);
    Loyalty = FMath::Clamp(Loyalty, 0.f, 100.f);
    Unrest = FMath::Clamp(Unrest, 0.f, 100.f);
    for (FDistrictData& District : Districts)
    {
        District.InfluenceValue = FMath::Clamp(District.InfluenceValue, 0.f, 100.f);
    }
}

void UCityPoliticalComponent::ModifyProsperity(float Delta)
{
    if (!GetOwner()->HasAuthority()) return;
    Prosperity += Delta;
    ClampValues();
}

void UCityPoliticalComponent::ModifyLoyalty(float Delta)
{
    if (!GetOwner()->HasAuthority()) return;
    Loyalty += Delta;
    ClampValues();
}

void UCityPoliticalComponent::ModifyUnrest(float Delta)
{
    if (!GetOwner()->HasAuthority()) return;
    Unrest += Delta;
    ClampValues();
}

void UCityPoliticalComponent::ModifyInfluence(int32 DistrictId, int32 GuildId, float Delta)
{
    if (!GetOwner()->HasAuthority()) return;

    for (FDistrictData& District : Districts)
    {
        if (District.DistrictId == DistrictId)
        {
            if (District.InfluenceGuildId == GuildId || District.InfluenceValue <= 0.f)
            {
                District.InfluenceGuildId = GuildId;
                District.InfluenceValue += Delta;
                ClampValues();
            }
            else
            {
                // Конкуренция: уменьшаем влияние текущего владельца
                District.InfluenceValue -= Delta;
                if (District.InfluenceValue <= 0.f && Delta > 0.f)
                {
                    District.InfluenceGuildId = GuildId;
                    District.InfluenceValue = FMath::Abs(Delta);
                }
                ClampValues();
            }
            break;
        }
    }
}
