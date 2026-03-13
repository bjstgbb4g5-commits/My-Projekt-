#include "ResourceSystem/FocusResource.h"

UFocusResource::UFocusResource()
{
}

void UFocusResource::Initialize(float InMax)
{
    Max = InMax;
    Current = Max;
}

bool UFocusResource::HasEnough(float Amount) const
{
    return Current >= Amount;
}

void UFocusResource::Spend(float Amount)
{
    Current = FMath::Max(0.f, Current - Amount);
}

void UFocusResource::Regenerate(float DeltaTime, bool bInCombat)
{
    Current = FMath::Min(Max, Current + Max * RegenRate * DeltaTime);
}

void UFocusResource::Gain(float Amount)
{
    Current = FMath::Min(Max, Current + Amount);
}
