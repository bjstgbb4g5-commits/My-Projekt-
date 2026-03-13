#pragma once

#include "CoreMinimal.h"
#include "LootTypes.generated.h"

UENUM(BlueprintType)
enum class ELootSourceType : uint8
{
    Mob,
    Elite,
    Boss,
    WorldBoss,
    Chest
};

USTRUCT(BlueprintType)
struct FLootEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ItemID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DropChance = 0.0f; // 0.0–1.0

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MinQuantity = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxQuantity = 1;
};

USTRUCT(BlueprintType)
struct FLootTable
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 TableID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FLootEntry> Entries;
};
