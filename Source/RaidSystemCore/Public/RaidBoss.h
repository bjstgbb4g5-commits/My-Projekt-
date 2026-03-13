#pragma once

#include "CoreMinimal.h"
#include "BossCharacter.h"
#include "RaidBoss.generated.h"

UCLASS()
class RAIDSYSTEMCORE_API ARaidBoss : public ABossCharacter
{
    GENERATED_BODY()

public:
    ARaidBoss();

    UPROPERTY(BlueprintReadOnly, Category = "Raid")
    int32 RaidID = -1;

    UFUNCTION(BlueprintCallable, Category = "Raid")
    void OnBossKilled();

protected:
    virtual void BeginPlay() override;
};
