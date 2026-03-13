#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestTypes.h"
#include "QuestDefinition.h"
#include "QuestComponent.generated.h"

USTRUCT()
struct FActiveQuestData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 QuestID = -1;

    // Прогресс по каждой цели (индекс соответствует индексу в FQuestDefinition::Objectives)
    UPROPERTY()
    TArray<int32> ObjectiveProgress;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTSYSTEMCORE_API UQuestComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UQuestComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Принять квест
    UFUNCTION(BlueprintCallable, Category = "Quest")
    bool AcceptQuest(int32 QuestID);

    // Обновить прогресс цели (вызывается из подсистемы)
    void UpdateObjective(int32 QuestID, int32 ObjectiveIndex, int32 Delta);

    // Завершить квест (вызывается из подсистемы при выполнении всех целей)
    void CompleteQuest(int32 QuestID);

    // Провалить квест
    void FailQuest(int32 QuestID);

    // Проверить, есть ли квест в активных
    UFUNCTION(BlueprintCallable, Category = "Quest")
    bool HasActiveQuest(int32 QuestID) const;

    // Получить список завершённых
    UFUNCTION(BlueprintCallable, Category = "Quest")
    const TArray<int32>& GetCompletedQuests() const { return CompletedQuests; }

    // Сохранить состояние (для PersistenceSystem)
    void SaveQuestData(FArchive& Ar);

    // Загрузить состояние
    void LoadQuestData(FArchive& Ar);

private:
    bool IsServer() const;

    UPROPERTY(Replicated)
    TMap<int32, FActiveQuestData> ActiveQuests;  // QuestID -> данные

    UPROPERTY(Replicated)
    TArray<int32> CompletedQuests;

    UPROPERTY(Replicated)
    TArray<int32> FailedQuests;

    // Проверка выполнения всех целей квеста
    bool CheckQuestCompletion(int32 QuestID) const;
};
