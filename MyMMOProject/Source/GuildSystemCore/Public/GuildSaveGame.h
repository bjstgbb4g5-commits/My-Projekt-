#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GuildTypes.h"
#include "GuildSaveGame.generated.h"

UCLASS()
class GUILDSYSTEMCORE_API UGuildSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<FGuildData> Guilds;

    UPROPERTY()
    int32 NextGuildId = 1;
};
