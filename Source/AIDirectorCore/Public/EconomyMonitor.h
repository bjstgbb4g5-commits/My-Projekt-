#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EconomyMonitor.generated.h"

class UAIWorldDirectorSubsystem;
class UMarketplaceSubsystem;

UCLASS()
class AIDIRECTORCORE_API UEconomyMonitor : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UAIWorldDirectorSubsystem* InDirector);
    void UpdateEconomy();

    bool ShouldIncreaseResource(const FString& ResourceName) const;

private:
    UPROPERTY()
    UAIWorldDirectorSubsystem* Director;

    UPROPERTY()
    UMarketplaceSubsystem* Marketplace;
};
