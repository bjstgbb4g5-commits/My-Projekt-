#pragma once

#include "CoreMinimal.h"
#include "HouseTypes.h"
#include "HousePermissions.generated.h"

UCLASS()
class HOUSESYSTEMCORE_API UHousePermissions : public UObject
{
    GENERATED_BODY()

public:
    static bool CanInviteMember(const FHouseData& HouseData, const FUniqueNetIdRepl& PlayerId);
    static bool CanKickMember(const FHouseData& HouseData, const FUniqueNetIdRepl& PlayerId);
    static bool CanUpgradeHouse(const FHouseData& HouseData, const FUniqueNetIdRepl& PlayerId);
    static bool CanManageTerritories(const FHouseData& HouseData, const FUniqueNetIdRepl& PlayerId);
};

inline bool UHousePermissions::CanInviteMember(const FHouseData& HouseData, const FUniqueNetIdRepl& PlayerId)
{
    for (const FHouseMember& Member : HouseData.Members)
    {
        if (Member.PlayerId == PlayerId)
        {
            return Member.Rank == EHouseRank::Leader || Member.Rank == EHouseRank::Officer;
        }
    }
    return false;
}

inline bool UHousePermissions::CanKickMember(const FHouseData& HouseData, const FUniqueNetIdRepl& PlayerId)
{
    for (const FHouseMember& Member : HouseData.Members)
    {
        if (Member.PlayerId == PlayerId)
        {
            return Member.Rank == EHouseRank::Leader || Member.Rank == EHouseRank::Officer;
        }
    }
    return false;
}

inline bool UHousePermissions::CanUpgradeHouse(const FHouseData& HouseData, const FUniqueNetIdRepl& PlayerId)
{
    for (const FHouseMember& Member : HouseData.Members)
    {
        if (Member.PlayerId == PlayerId)
        {
            return Member.Rank == EHouseRank::Leader;
        }
    }
    return false;
}

inline bool UHousePermissions::CanManageTerritories(const FHouseData& HouseData, const FUniqueNetIdRepl& PlayerId)
{
    for (const FHouseMember& Member : HouseData.Members)
    {
        if (Member.PlayerId == PlayerId)
        {
            return Member.Rank == EHouseRank::Leader;
        }
    }
    return false;
}
