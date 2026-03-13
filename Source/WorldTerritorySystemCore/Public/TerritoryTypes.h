#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "TerritoryTypes.generated.h"

UENUM(BlueprintType)
enum class ETerritoryType : uint8
{
    Wilderness,
    ResourceZone,
    CityDistrict,
    DungeonEntrance
};

UENUM(BlueprintType)
enum class EResourceType : uint8
{
    None,
    Wood,
    Stone,
    Iron,
    Silver,
    Crystal,
    Artifact
};

USTRUCT(BlueprintType)
struct FResourceDeposit
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EResourceType ResourceType = EResourceType::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxDeposit = 100000;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 CurrentDeposit = 100000;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float RegenerationRate = 0.01f;
};

USTRUCT(BlueprintType)
struct FTerritoryData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 TerritoryID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString TerritoryName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ETerritoryType TerritoryType = ETerritoryType::Wilderness;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName OwnerGuild;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float TaxRate = 0.05f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 FortificationLevel = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bUnderWar = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FResourceDeposit Resource;
};
