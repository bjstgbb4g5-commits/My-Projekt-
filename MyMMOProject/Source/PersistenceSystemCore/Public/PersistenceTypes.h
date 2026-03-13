#pragma once

#include "CoreMinimal.h"
#include "PersistenceTypes.generated.h"

USTRUCT()
struct FPlayerSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    FUniqueNetIdRepl PlayerId;

    // Progression – упрощённо, на самом деле нужно сохранять все карты
    UPROPERTY()
    TMap<int32, float> WeaponMastery;

    // Inventory – сериализовать как массив байт (или напрямую)
    UPROPERTY()
    TArray<uint8> InventoryData; // в реальности нужно сериализовать FInventoryContainer

    // Economy
    UPROPERTY()
    int32 Gold;

    // Mounts
    UPROPERTY()
    TArray<int32> OwnedMountIDs;

    // Guild
    UPROPERTY()
    int32 GuildId;

    // Добавляем операторы сериализации
    friend FArchive& operator<<(FArchive& Ar, FPlayerSaveData& Data)
    {
        Ar << Data.PlayerId;
        Ar << Data.WeaponMastery;
        Ar << Data.InventoryData;
        Ar << Data.Gold;
        Ar << Data.OwnedMountIDs;
        Ar << Data.GuildId;
        return Ar;
    }
};
