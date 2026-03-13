#pragma once

#include "CoreMinimal.h"
#include "ResourceTypes.generated.h"

UENUM(BlueprintType)
enum class EWorldResourceType : uint8
{
    Wood,
    Stone,
    Iron,
    Silver,
    Crystal,
    Artifact
};

UENUM(BlueprintType)
enum class EResourceNodeState : uint8
{
    Available,
    Depleted,
    Regenerating
};

USTRUCT(BlueprintType)
struct FResourceNode
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, - BlueprintReadOnly)
    int32 NodeID =1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EWorldResourceType ResourceType = EWorldResourceType::Wood;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxAmount = 1000;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 CurrentAmount = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float RegenerationRate = 0.02f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EResourceNodeState State = EResourceNodeState::Available;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 TerritoryID = -1;
};
