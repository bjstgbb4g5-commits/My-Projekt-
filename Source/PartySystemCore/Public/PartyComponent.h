#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PartyTypes.h"
#include "PartyComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PARTYSYSTEMCORE_API UPartyComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPartyComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category = "Party")
    void SetParty(int32 InPartyId, EPartyRole InRole);

    UFUNCTION(BlueprintCallable, Category = "Party")
    int32 GetPartyId() const { return PartyId; }

    UFUNCTION(BlueprintCallable, Category = "Party")
    EPartyRole GetRole() const { return Role; }

protected:
    UPROPERTY(Replicated)
    int32 PartyId;

    UPROPERTY(Replicated)
    EPartyRole Role;
};
