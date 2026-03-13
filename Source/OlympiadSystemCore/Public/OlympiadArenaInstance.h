#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OlympiadTypes.h"
#include "OlympiadArenaInstance.generated.h"

UCLASS()
class OLYMPIADSYSTEMCORE_API AOlympiadArenaInstance : public AActor
{
    GENERATED_BODY()

public:
    AOlympiadArenaInstance();

    UFUNCTION(BlueprintCallable, Category = "Olympiad")
    void InitializeArena(int32 MatchID, APlayerController* PlayerA, APlayerController* PlayerB);

    UFUNCTION(BlueprintCallable, Category = "Olympiad")
    void StartFight();

    UFUNCTION(BlueprintCallable, Category = "Olympiad")
    void EndFight(APlayerController* Winner);

    UPROPERTY(Replicated, BlueprintReadOnly)
    int32 CurrentMatchID;

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY()
    APlayerController* ContestantA;

    UPROPERTY()
    APlayerController* ContestantB;

    FTimerHandle FightTimerHandle;
    void OnFightTimeout();
};
