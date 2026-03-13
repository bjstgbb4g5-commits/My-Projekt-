#pragma once

#include "CoreMinimal.h"
#include "WeaponMasteryTypes.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Sword,
    Mace,
    DualDaggers,
    DualSwords,
    Greatsword,
    Spear,
    Hammer,
    Bow,
    Crossbow,
    MageStaff,
    SummonerStaff,
    PriestStaff,
    MAX
};

UENUM(BlueprintType)
enum class EMasteryRank : uint8
{
    Novice = 1,
    Apprentice,
    Fighter,
    Veteran,
    Warrior,
    Champion,
    Master,
    Grandmaster,
    Legend,
    Archmaster
};

USTRUCT(BlueprintType)
struct FWeaponMasteryData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EWeaponType WeaponType = EWeaponType::Sword;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 MasteryLevel = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float MasteryXP = 0.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<int32> UnlockedSkills;
};
