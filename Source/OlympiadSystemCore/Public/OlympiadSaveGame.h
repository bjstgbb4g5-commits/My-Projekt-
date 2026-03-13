#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OlympiadTypes.h"
#include "OlympiadSaveGame.generated.h"

UCLASS()
class OLYMPIADSYSTEMCORE_API UOlympiadSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TMap<FUniqueNetIdRepl, FOlympiadPlayerData> PlayerRatings;

    UPROPERTY()
    TArray<FOlympiadPlayerData> ChampionsHistory; // последние чемпионы

    UPROPERTY()
    int32 NextMatchID = 1;

    UPROPERTY()
    FDateTime LastWeeklyReset;
};
