#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemTypes.h"
#include "ItemStats.h"
#include "ItemDefinition.generated.h"

USTRUCT(BlueprintType)
struct FItemDefinition : public FTableRowBase
{
    GENERATED_BODY()

    // уникальный ID предмета
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ItemID = -1;

    // название
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName Name;

    // тип предмета
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EItemType Type = EItemType::Resource;

    // базовая редкость (может быть изменена при создании)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EItemRarity Rarity = EItemRarity::Gray;

    // максимальный размер стопки
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxStack = 1;

    // можно ли стакать (если нет, MaxStack = 1)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bStackable = true;

    // можно ли экипировать
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bEquippable = false;

    // тип оружия (если оружие)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EWeaponType WeaponType = EWeaponType::Sword;

    // тип брони (если броня)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EArmorType ArmorType = EArmorType::Cloth;

    // базовые статы
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FItemStats Stats;

    // максимальная прочность (0 = неразрушаемый)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 DurabilityMax = 100;
};
