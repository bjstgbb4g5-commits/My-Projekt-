#pragma once

#include "CoreMinimal.h"
#include "ResourceSystem/IResourceSystem.h"
#include "ManaResource.generated.h"

UCLASS(BlueprintType)
class COMBATCORE_API UManaResource : public UObject, public IResourceSystem
{
    GENERATED_BODY()

public:
    UManaResource();

    virtual EResourceType GetResourceType() const override { return EResourceType::Mana; }
    virtual float GetCurrent() const override { return Current; }
    virtual float GetMax() const override { return Max; }
    virtual bool HasEnough(float Amount) const override;
    virtual void Spend(float Amount) override;
    virtual void Regenerate(float DeltaTime, bool bInCombat) override;
    virtual void Gain(float Amount) override;

    void Initialize(float InMax);

private:
    float Current = 0.f;
    float Max = 100.f;
    float RegenCombat = 0.03f;
    float RegenOutOfCombat = 0.08f;
};
