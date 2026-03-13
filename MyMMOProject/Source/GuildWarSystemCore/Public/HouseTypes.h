#pragma once

#include "CoreMinimal.h"
#include "HouseTypes.generated.h"

UENUM(BlueprintType)
enum class EHouseType : uint8
{
    Hall,
    Outpost,
    Fortress
};

UENUM(BlueprintType)
enum class EHouseRank : uint8
{
    Recruit,
    Member,
    Veteran,
    Officer,
    Leader
};

USTRUCT(BlueprintType)
struct FHouseMember
{
    GENERATED_BODY()

    UPROPERTY()
    FUniqueNetIdRepl PlayerId;

    UPROPERTY()
    EHouseRank Rank = EHouseRank::Recruit;

    UPROPERTY()
    FDateTime JoinedTime;
};

USTRUCT(BlueprintType)
struct FHouseData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 HouseId = -1;

    UPROPERTY()
    FString HouseName;

    UPROPERTY()
    FUniqueNetIdRepl FounderPlayerId;

    UPROPERTY()
    int32 GuildId = -1;

    UPROPERTY()
    int32 Level = 1;

    UPROPERTY()
    EHouseType HouseType = EHouseType::Hall;

    UPROPERTY()
    FVector Location;

    UPROPERTY()
    TArray<FHouseMember> Members;

    UPROPERTY()
    TArray<int32> OwnedTerritories;

    UPROPERTY()
    FDateTime CreatedTime;

    UPROPERTY()
    int32 MaxMembers = 200; // увеличен до 200, лимит из константы
};
