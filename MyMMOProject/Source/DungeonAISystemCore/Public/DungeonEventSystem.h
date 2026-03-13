#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonAITypes.h"
#include "DungeonEventSystem.generated.h"

UCLASS()
class DUNGEONAISYSTEMCORE_API UDungeonEventSystem : public UObject
{
    GENERATED_BODY()

public:
    void EvaluateEvents();

private:
    EDungeonEventType ChooseEvent();
    void SpawnAmbush();
    void SpawnElitePatrol();
    void SpawnTreasureGoblin();
};
