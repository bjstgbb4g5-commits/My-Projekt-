#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IResourceSystem.generated.h"

UINTERFACE(MinimalAPI)
class UResourceSystem : public UInterface
{
    GENERATED_BODY()
};

class IResourceSystem
{
    GENERATED_BODY()

public:
    virtual EResourceType GetResourceType() const = 0;
    virtual float GetCurrent() const = 0;
    virtual float GetMax() const = 0;
    virtual bool HasEnough(float Amount) const = 0;
    virtual void Spend(float Amount) = 0;
    virtual void Regenerate(float DeltaTime, bool bInCombat) = 0;
    virtual void Gain(float Amount) = 0;
};
