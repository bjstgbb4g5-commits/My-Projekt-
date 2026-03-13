#pragma once

#include "CoreMinimal.h"
#include "ChatTypes.generated.h"

UENUM(BlueprintType)
enum class EChatChannel : uint8
{
    Local,
    Party,
    Guild,
    Trade,
    Global,
    System,
    Whisper
};
