#pragma once

#include "CoreMinimal.h"
#include "CharacterStats.generated.h"

UENUM(BlueprintType)
enum class EElementType : uint8
{
    None,
    Fire,
    Water,
    Earth,
    Wind,
    Metal
};

UENUM(BlueprintType)
enum class EResourceType : uint8
{
    None,
    Rage,
    Mana,
    Focus,
    Essence
};

USTRUCT(BlueprintType)
struct FCharacterStats
{
    GENERATED_BODY()

    // Основные статы
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Power = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Defense = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Vitality = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Haste = 0.f; // 0..1

    // Боевые статы
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (ClampMin = 0, ClampMax = 1))
    float CritChance = 0.05f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float CritMultiplier = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (ClampMin = 0, ClampMax = 1))
    float ControlPower = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (ClampMin = 0, ClampMax = 1))
    float ControlResist = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Mobility = 1.f;

    // Текущие значения (не редактируются, только для отображения)
    float CurrentHealth;
};
