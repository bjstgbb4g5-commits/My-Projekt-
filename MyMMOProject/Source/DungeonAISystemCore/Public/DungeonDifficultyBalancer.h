#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonAITypes.h"
#include "DungeonDifficultyBalancer.generated.h"

UCLASS()
class DUNGEONAISYSTEMCORE_API UDungeonDifficultyBalancer : public UObject
{
    GENERATED_BODY()

public:
    float GetHealthMultiplier(const FDungeonGroupProfile& Profile);
    float GetDamageMultiplier(const FDungeonGroupProfile& Profile);
};
