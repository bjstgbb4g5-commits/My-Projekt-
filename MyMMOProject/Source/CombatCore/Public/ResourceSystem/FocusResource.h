#pragma once

#include "CoreMinimal.h"
#include "ResourceSystem/IResourceSystem.h"
#include "FocusResource.generated.h"

UCLASS(BlueprintType)
class COMBATCORE_API UFocusResource : public UObject, public IResourceSystem
{
    GENERATED_BODY()

public:
    UFocusResource();

    virtual EResourceType GetResourceType() const override { return EResourceType::Focus; }
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
    float RegenRate = 0.12f;
};
