#pragma once

#include "CoreMinimal.h"
#include "ResourceSystem/IResourceSystem.h"
#include "EssenceResource.generated.h"

UCLASS(BlueprintType)
class COMBATCORE_API UEssenceResource : public UObject, public IResourceSystem
{
    GENERATED_BODY()

public:
    UEssenceResource();

    virtual EResourceType GetResourceType() const override { return EResourceType::Essence; }
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
    float DecayRate = 1.f;
};
