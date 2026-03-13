#pragma once

#include "CoreMinimal.h"
#include "PoliticalEnums.generated.h"

UENUM(BlueprintType)
enum class EPoliticalRank : uint8
{
    Citizen,
    GuildMember,
    GuildLeader,
    Mayor,
    Elite
};

UENUM(BlueprintType)
enum class ECityDoctrine : uint8
{
    None,
    Military,
    Diplomatic,
    Progressive
};
