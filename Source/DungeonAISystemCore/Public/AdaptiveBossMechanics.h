#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonAITypes.h"
#include "AdaptiveBossMechanics.generated.h"

UCLASS()
class DUNGEONAISYSTEMCORE_API UAdaptiveBossMechanics : public UObject
{
    GENERATED_BODY()

public:
    TArray<int32> ChooseBossAbilities(int32 BossID, const FDungeonGroupProfile& Profile);

private:
    static constexpr int32 MAX_ABILITIES = 8;
};
