#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonTypes.h"
#include "DungeonKey.generated.h"

USTRUCT(BlueprintType)
struct FDungeonKey
{
    GENERATED_BODY()

    UPROPERTY()
    int32 DungeonID = -1;

    UPROPERTY()
    EDungeonDifficulty Difficulty = EDungeonDifficulty::Normal;
};

UCLASS()
class DUNGEONSYSTEMCORE_API UDungeonKeyManager : public UObject
{
    GENERATED_BODY()

public:
    // Заглушка для управления ключами
};
