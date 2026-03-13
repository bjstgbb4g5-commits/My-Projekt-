#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PvETypes.h"
#include "PvEZoneSubsystem.generated.h"

UCLASS()
class PVEWORLDCORE_API UPvEZoneSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    EPvEZoneType GetZoneTypeAtLocation(FVector Location) const;
};
