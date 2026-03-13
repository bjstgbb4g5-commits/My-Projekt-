#pragma once

#include "CoreMinimal.h"
#include "BossCharacter.h"
#include "DungeonBoss.generated.h"

UCLASS()
class DUNGEONSYSTEMCORE_API ADungeonBoss : public ABossCharacter
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Dungeon")
    int32 DungeonInstanceID = -1;

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    void OnBossKilled();
};
