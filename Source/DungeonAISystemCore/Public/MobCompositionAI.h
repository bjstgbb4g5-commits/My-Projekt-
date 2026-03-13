#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonAITypes.h"
#include "MobCompositionAI.generated.h"

UCLASS()
class DUNGEONAISYSTEMCORE_API UMobCompositionAI : public UObject
{
    GENERATED_BODY()

public:
    EDungeonMobRole ChooseMobRole(const FDungeonGroupProfile& Profile);
};
