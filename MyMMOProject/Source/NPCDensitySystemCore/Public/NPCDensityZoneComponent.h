#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCDensityTypes.h"
#include "NPCDensityZoneComponent.generated.h"

class UNPCDensityManager;

UCLASS(ClassGroup=(NPC), meta=(BlueprintSpawnableComponent))
class NPCDENSITYSYSTEMCORE_API UNPCDensityZoneComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNPCDensityZoneComponent();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditAnywhere, Category = "Density")
    FNPCDensityZoneConfig Config;

    int32 GetZoneId() const { return ZoneId; }
    FVector GetZoneCenter() const { return GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector; }
    float GetZoneRadius() const { return Config.SpawnRadius; }

private:
    friend class UNPCDensityManager;
    int32 ZoneId = -1;
};
