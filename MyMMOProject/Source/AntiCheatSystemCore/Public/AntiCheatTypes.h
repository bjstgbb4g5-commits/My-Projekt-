#pragma once

#include "CoreMinimal.h"
#include "AntiCheatTypes.generated.h"

UENUM(BlueprintType)
enum class ECheatType : uint8
{
    None,
    SpeedHack,
    DamageHack,
    TeleportHack,
    EconomyHack,
    Botting,
    PacketInjection,
    AbilitySpam
};

USTRUCT(BlueprintType)
struct FPlayerSuspicionData
{
    GENERATED_BODY()

    UPROPERTY()
    FUniqueNetIdRepl PlayerID;

    UPROPERTY()
    int32 SuspicionScore = 0;

    UPROPERTY()
    int32 SpeedFlags = 0;

    UPROPERTY()
    int32 DamageFlags = 0;

    UPROPERTY()
    int32 TeleportFlags = 0;

    UPROPERTY()
    int32 EconomyFlags = 0;

    UPROPERTY()
    int32 BotFlags = 0;

    UPROPERTY()
    int32 PacketFlags = 0;

    UPROPERTY()
    int32 AbilitySpamFlags = 0;

    UPROPERTY()
    FDateTime LastUpdate;

    UPROPERTY()
    float LastRecordedSpeed = 0.f;

    UPROPERTY()
    FVector LastLocation;

    UPROPERTY()
    FDateTime LastActionTime;
};
