#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PersistenceTypes.h"
#include "PlayerSaveGame.generated.h"

UCLASS()
class PERSISTENCESYSTEMCORE_API UPlayerSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    FPlayerSaveData PlayerData;
};
