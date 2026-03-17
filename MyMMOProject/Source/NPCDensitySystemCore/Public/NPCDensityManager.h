#pragma once

#include "CoreMinimal.h"
#include "NPCDensityTypes.h"
#include "NPCDensityZoneComponent.h"
#include "NPCLifeSimulationCore/Public/NPCLifeComponent.h"
#include "NPCDensityManager.generated.h"

class UNPCLifeSubsystem;

UCLASS()
class NPCDENSITYSYSTEMCORE_API UNPCDensityManager : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UWorld* InWorld);
    void Shutdown();

    void RegisterZone(UNPCDensityZoneComponent* Zone);
    void UnregisterZone(UNPCDensityZoneComponent* Zone);

    void UpdateZones(float DeltaTime, float CurrentTime);

    AActor* AcquireNPC(TSubclassOf<AActor> NPCClass, const FVector& Location, const FNPCDensityZoneConfig& ZoneConfig);
    void ReleaseNPC(AActor* NPC);

    int32 GetActiveCountInZone(int32 ZoneId) const;

    static ENPCLODLevel ComputeLOD(AActor* NPCActor, UWorld* World);

private:
    struct FZoneData
    {
        UNPCDensityZoneComponent* ZoneComponent;
        TArray<AActor*> ActiveNPCs;
    };

    UPROPERTY()
    UWorld* World;

    UPROPERTY()
    UNPCLifeSubsystem* NPCLifeSubsystem;

    TMap<int32, FZoneData> Zones;
    int32 NextZoneId = 0;

    TMap<UClass*, TArray<FNPCPoolEntry>> Pool;

    void UpdateZone(FZoneData& ZoneData, float CurrentTime);
    void UpdateLODForNPC(AActor* NPC);
    void SpawnNPCInZone(FZoneData& ZoneData);
    void DespawnNPCFromZone(FZoneData& ZoneData, AActor* NPC);

    FVector FindGroundLocation(const FVector& StartLocation, UWorld* World) const;
};
