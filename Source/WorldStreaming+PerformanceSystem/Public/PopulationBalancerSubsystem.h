#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PopulationBalancerSubsystem.generated.h"

UCLASS()
class WORLDPERFORMANCECORE_API UPopulationBalancerSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UPopulationBalancerSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
    bool IsServer() const;
    void CheckRegionOverload();

    FTSTicker::FDelegateHandle TickHandle;
    bool Tick(float DeltaTime);
};
