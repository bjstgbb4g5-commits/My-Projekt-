#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ChronicleTypes.h"
#include "ChronicleSaveGame.generated.h"

UCLASS()
class CHRONICLESYSTEMCORE_API UChronicleSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    EChronicleID Chronicle = EChronicleID::Chronicle1;

    UPROPERTY()
    int32 CurrentSeason = 1;

    UPROPERTY()
    FDateTime SeasonStartTime;

    UPROPERTY()
    bool bNoxDefeated = false;
};
