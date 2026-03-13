#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestManager.generated.h"

class UQuestSubsystem;

UCLASS()
class QUESTSYSTEMCORE_API UQuestManager : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UQuestSubsystem* InSubsystem);

    // Подписки на события из других систем
    void RegisterDelegates();
    void UnregisterDelegates();

private:
    UPROPERTY()
    UQuestSubsystem* QuestSubsystem;

    // Обработчики событий
    UFUNCTION()
    void OnEnemyKilled(AActor* Killer, AActor* Victim, int32 EnemyID);

    UFUNCTION()
    void OnItemCollected(APlayerState* Player, int32 ItemID, int32 Quantity);

    UFUNCTION()
    void OnLocationVisited(APlayerState* Player, int32 RegionID);

    UFUNCTION()
    void OnNPCInteracted(APlayerState* Player, int32 NPCID);

    UFUNCTION()
    void OnCraftCompleted(APlayerState* Player, int32 ItemID);

    UFUNCTION()
    void OnWorldEventParticipated(APlayerState* Player, int32 EventID);
};
