#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoliticalEnums.h"
#include "PoliticalIdentityComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CITYPOLITICSCORE_API UPoliticalIdentityComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPoliticalIdentityComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Political")
    EPoliticalRank PoliticalRank;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Political")
    int32 ConsecutiveMayorTerms;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Political")
    int32 RegisteredCityId;
};
