#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterStats.h"
#include "ElementMatrix.generated.h"

USTRUCT(BlueprintType)
struct FElementPair
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EElementType Attacker;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EElementType Defender;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Multiplier = 1.0f;
};

UCLASS(BlueprintType)
class COMBATCORE_API UElementMatrix : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FElementPair> Modifiers;

    UFUNCTION(BlueprintCallable, Category = "Element")
    float GetModifier(EElementType Attacker, EElementType Defender) const;

private:
    mutable TMap<TPair<EElementType, EElementType>, float> Lookup;
    void BuildLookup() const;
};
