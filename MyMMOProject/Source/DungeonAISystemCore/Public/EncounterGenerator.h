#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonAITypes.h"
#include "EncounterGenerator.generated.h"

UCLASS()
class DUNGEONAISYSTEMCORE_API UEncounterGenerator : public UObject
{
    GENERATED_BODY()

public:
    void UpdateEncounters();

    FDungeonEncounter GenerateEncounter(const FDungeonGroupProfile& Profile);

private:
    static constexpr int32 BASE_MOBS = 5;
};
