#include "ResourceSystem/EssenceResource.h"

UEssenceResource::UEssenceResource()
{
}

void UEssenceResource::Initialize(float InMax)
{
    Max = InMax;
    Current = 0.f;
}

bool UEssenceResource::HasEnough(float Amount) const
{
    return Current >= Amount;
}

void UEssenceResource::Spend(float Amount)
{
    Current = FMath::Max(0.f, Current - Amount);
}

void UEssenceResource::Regenerate(float DeltaTime, bool bInCombat)
{
    if (!bInCombat)
    {
        Current = FMath::Max(0.f, Current - DecayRate * DeltaTime);
    }
}

void UEssenceResource::Gain(float Amount)
{
    Current = FMath::Min(Max, Current + Amount);
}
