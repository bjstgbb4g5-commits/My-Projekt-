#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PerformanceTypes.h"
#include "WorldGeneratorCore/Public/WorldGeneratorSubsystem.h"
#include "RegionStreamingSubsystem.generated.h"

UCLASS()
class WORLDPERFORMANCECORE_API URegionStreamingSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    URegionStreamingSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Streaming")
    void UpdateRegionStreaming(FVector PlayerLocation);

    UFUNCTION(BlueprintCallable, Category = "Streaming")
    int32 GetLoadedRegionCount() const { return RegionRuntime.Num(); }

private:
    bool IsServer() const;
    void LoadRegion(int32 RegionIndex);
    void UnloadRegion(int32 RegionIndex);

    UPROPERTY()
    UWorldGeneratorSubsystem* WorldGen;

    UPROPERTY()
    TMap<int32, FRegionRuntimeState> RegionRuntime;

    FTSTicker::FDelegateHandle TickHandle;
    bool Tick(float DeltaTime);
};
