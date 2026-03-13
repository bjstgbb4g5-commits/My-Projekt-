#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "QuestDefinition.h"
#include "QuestTypes.h"
#include "QuestSubsystem.generated.h"

class UQuestComponent;
class UQuestManager;

// Ключ для TargetIndex
USTRUCT()
struct FQuestTargetKey
{
    GENERATED_BODY()

    UPROPERTY()
    EQuestObjectiveType Type;

    UPROPERTY()
    int32 TargetID;

    friend uint32 GetTypeHash(const FQuestTargetKey& Key)
    {
        return HashCombine(GetTypeHash(Key.Type), GetTypeHash(Key.TargetID));
    }

    bool operator==(const FQuestTargetKey& Other) const
    {
        return Type == Other.Type && TargetID == Other.TargetID;
    }
};

// Запись в TargetIndex
USTRUCT()
struct FQuestTargetIndexEntry
{
    GENERATED_BODY()

    UPROPERTY()
    FUniqueNetIdRepl PlayerId;

    UPROPERTY()
    int32 QuestID = -1;

    UPROPERTY()
    int32 ObjectiveIndex = -1;
};

UCLASS()
class QUESTSYSTEMCORE_API UQuestSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UQuestSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Загрузить таблицу квестов
    void LoadQuestDefinitions();

    // Получить определение квеста
    UFUNCTION(BlueprintCallable, Category = "Quest")
    const FQuestDefinition* GetQuestDefinition(int32 QuestID) const;

    // Зарегистрировать игрока и его квесты в индексе
    void RegisterPlayerQuests(APlayerState* PlayerState, UQuestComponent* QuestComp, bool bAdd);

    // Полное удаление игрока при логауте
    void UnregisterPlayer(APlayerState* PlayerState);

    // Обновить прогресс цели для всех игроков, у которых есть активный квест на эту цель
    void NotifyObjectiveProgress(AActor* TargetActor, EQuestObjectiveType Type, int32 TargetID, int32 Delta = 1);

    // Выдать награды за квест
    void GrantRewards(AActor* PlayerActor, int32 QuestID);

    // Проверка условий квеста (уровень, хроника и т.д.)
    bool ValidateQuestPrerequisites(AActor* PlayerActor, const FQuestDefinition& Def) const;

    // Обработчики событий от QuestManager
    void OnEnemyKilled(AActor* Killer, AActor* Victim, int32 EnemyID);
    void OnItemCollected(APlayerState* Player, int32 ItemID, int32 Quantity);
    void OnLocationVisited(APlayerState* Player, int32 RegionID);
    void OnNPCInteracted(APlayerState* Player, int32 NPCID);
    void OnCraftCompleted(APlayerState* Player, int32 ItemID);
    void OnWorldEventParticipated(APlayerState* Player, int32 EventID);

private:
    UPROPERTY()
    UDataTable* QuestTable;

    TMap<int32, FQuestDefinition> QuestDefinitions;

    // Индекс: ключ (тип + ID цели) -> список записей (игрок, квест, индекс цели)
    TMap<FQuestTargetKey, TArray<FQuestTargetIndexEntry>> TargetIndex;

    // Соответствие PlayerId -> QuestComponent (для быстрого доступа при удалении)
    TMap<FUniqueNetIdRepl, TWeakObjectPtr<UQuestComponent>> PlayerQuestComponents;

    UPROPERTY()
    UQuestManager* QuestManager;

    // Вспомогательная функция для построения ключа
    FQuestTargetKey MakeKey(EQuestObjectiveType Type, int32 TargetID) const;
};
