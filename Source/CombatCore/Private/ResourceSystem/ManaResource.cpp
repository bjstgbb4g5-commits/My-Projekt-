#include "ResourceSystem/ManaResource.h"

UManaResource::UManaResource()
{
}

void UManaResource::Initialize(float InMax)
{
    Max = InMax;
    Current = Max;
}

bool UManaResource::HasEnough(float Amount) const
{
    return Current >= Amount;
}

void UManaResource::Spend(float Amount)
{
    Current = FMath::Max(0.f, Current - Amount);
}

void UManaResource::Regenerate(float DeltaTime, bool bInCombat)
{
    float Rate = bInCombat ? RegenCombat : RegenOutOfCombat;
    Current = FMath::Min(Max, Current + Max * Rate * DeltaTime);
}

void UManaResource::Gain(float Amount)
{
    Current = FMath::Min(Max, Current + Amount);
}
