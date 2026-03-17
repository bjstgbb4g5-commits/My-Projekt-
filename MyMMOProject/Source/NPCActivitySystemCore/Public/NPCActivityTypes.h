#pragma once

#include "CoreMinimal.h"
#include "NPCActivityTypes.generated.h"

UENUM(BlueprintType)
enum class ENPCActivityType : uint8
{
    None,
    Sit,
    Talk,
    Work,
    Eat,
    Train,
    Relax
};

USTRUCT(BlueprintType)
struct FActivitySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    ENPCActivityType ActivityType = ENPCActivityType::None;

    UPROPERTY(EditAnywhere)
    int32 MaxOccupancy = 1;

    TWeakObjectPtr<AActor> OccupyingNPC;
};
