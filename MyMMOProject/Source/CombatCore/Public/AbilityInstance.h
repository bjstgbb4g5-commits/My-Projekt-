#pragma once

#include "CoreMinimal.h"
#include "AbilityDataAsset.h"
#include "AbilityInstance.generated.h"

UENUM(BlueprintType)
enum class EAbilityOrigin : uint8
{
    Class,
    Weapon,
    Profession,
    Guild
};

USTRUCT(BlueprintType)
struct FAbilityInstance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAbilityDataAsset* BaseData = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Rank = 1; // 1..8

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAbilityOrigin Origin = EAbilityOrigin::Class;

    float GetPower() const
    {
        return BaseData ? BaseData->BaseSkillPower * (1.f + 0.1f * (Rank - 1)) : 0.f;
    }

    float GetResourceCost() const
    {
        return BaseData ? BaseData->ResourceCost * (1.f - 0.03f * (Rank - 1)) : 0.f;
    }

    float GetCooldown() const
    {
        return BaseData ? BaseData->Cooldown * (1.f - 0.05f * (Rank - 1)) : 0.f;
    }
};
