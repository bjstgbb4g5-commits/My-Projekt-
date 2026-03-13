#pragma once

#include "CoreMinimal.h"
#include "GuildCareerTypes.generated.h"

UENUM(BlueprintType)
enum class EGuildType : uint8
{
    Shadow,
    Industrial,
    Military,
    TradeAgrarian,
    Arcane,
    Administrative
};

UENUM(BlueprintType)
enum class EGuildRank : uint8
{
    Recruit,
    Member,
    Veteran,
    Officer,
    Master,
    GuildLeader
};

USTRUCT(BlueprintType)
struct FGuildCareerData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EGuildType Guild = EGuildType::Shadow;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EGuildRank Rank = EGuildRank::Recruit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Progress = 0.f;
};
