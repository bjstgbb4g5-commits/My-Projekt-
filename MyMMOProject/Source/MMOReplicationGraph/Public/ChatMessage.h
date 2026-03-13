#pragma once

#include "CoreMinimal.h"
#include "ChatTypes.h"
#include "ChatMessage.generated.h"

USTRUCT(BlueprintType)
struct FChatMessage
{
    GENERATED_BODY()

    UPROPERTY()
    FUniqueNetIdRepl SenderId;

    UPROPERTY()
    FString SenderName;

    UPROPERTY()
    EChatChannel Channel;

    UPROPERTY()
    FString MessageText;

    UPROPERTY()
    FDateTime Timestamp;

    UPROPERTY()
    FUniqueNetIdRepl TargetPlayerId; // for whispers
};
