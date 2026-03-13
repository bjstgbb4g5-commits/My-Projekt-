#pragma once

#include "CoreMinimal.h"
#include "WorldGeneratorTypes.generated.h"

UENUM(BlueprintType)
enum class EBiomeType : uint8
{
    Plains,
    Forest,
    Mountains,
    Volcano,
    Arcane,
    Shadow,
    Ruins
};

UENUM(BlueprintType)
enum class EZoneType : uint8
{
    Safe,
    Conflict,
    Black
};

USTRUCT(BlueprintType)
struct FWorldRegion
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 RegionX = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 RegionY = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EBiomeType Biome = EBiomeType::Plains;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool HasCity = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool HasDungeon = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool HasVillage = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float AverageHeight = 0.f;
};

USTRUCT(BlueprintType)
struct FPOIData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FVector Location;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName Type; // "BanditCamp", "Ruins", "ResourceNode", "QuestCamp", "Shrine"
};

USTRUCT(BlueprintType)
struct FZoneData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FVector Center;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Radius;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EZoneType Type;
};

USTRUCT(BlueprintType)
struct FCitySpawnData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector Location;
};
