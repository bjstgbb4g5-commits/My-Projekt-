#include "ElementMatrix.h"

float UElementMatrix::GetModifier(EElementType Attacker, EElementType Defender) const
{
    if (Lookup.Num() == 0)
        BuildLookup();

    const float* Found = Lookup.Find(TPair<EElementType, EElementType>(Attacker, Defender));
    return Found ? *Found : 1.0f;
}

void UElementMatrix::BuildLookup() const
{
    Lookup.Empty();
    for (const FElementPair& Pair : Modifiers)
    {
        Lookup.Add(TPair<EElementType, EElementType>(Pair.Attacker, Pair.Defender), Pair.Multiplier);
    }
}
