#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LootTypes.h"
#include "LootSaveGame.generated.h"

UCLASS()
class LOOTSYSTEMCORE_API ULootSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TMap<int32, FLootTable> LootTables; // может, не нужно сохранять, так как таблицы статичны
};
