#include "ResourceSystem/RageResource.h"

URageResource::URageResource()
{
}

void URageResource::Initialize(float InMax)
{
    Max = InMax;
    Current = 0.f;
}

bool URageResource::HasEnough(float Amount) const
{
    return Current >= Amount;
}

void URageResource::Spend(float Amount)
{
    Current = FMath::Max(0.f, Current - Amount);
}

void URageResource::Regenerate(float DeltaTime, bool bInCombat)
{
    if (!bInCombat)
    {
        Current = FMath::Max(0.f, Current - DecayRate * DeltaTime);
    }
    // В бою не регенерируется, только через удары
}

void URageResource::Gain(float Amount)
{
    Current = FMath::Min(Max, Current + Amount);
}
