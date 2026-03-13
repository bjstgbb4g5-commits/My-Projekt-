#pragma once

#include "CoreMinimal.h"
#include "JusticeEnums.generated.h"

UENUM(BlueprintType)
enum class EJusticeState : uint8
{
    None,
    Marked,
    Imprisoned,
    AwaitingExecution,
    ExecutionDebuff
};
