#include "QuestComponent.h"
#include "QuestSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

UQuestComponent::UQuestComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UQuestComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UQuestComponent, ActiveQuests, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UQuestComponent, CompletedQuests, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UQuestComponent, FailedQuests, COND_OwnerOnly);
}

bool UQuestComponent::IsServer() const
{
    return GetOwner() && GetOwner()->HasAuthority();
}

bool UQuestComponent::HasActiveQuest(int32 QuestID) const
{
    return ActiveQuests.Contains(QuestID);
}

bool UQuestComponent::AcceptQuest(int32 QuestID)
{
    if (!IsServer()) return false;

    // Нельзя принять уже завершённый
    if (CompletedQuests.Contains(QuestID))
        return false;

    // Уже активен?
    if (ActiveQuests.Contains(QuestID))
        return false;

    UQuestSubsystem* QuestSys = GetWorld()->GetSubsystem<UQuestSubsystem>();
    if (!QuestSys) return false;

    const FQuestDefinition* Def = QuestSys->GetQuestDefinition(QuestID);
    if (!Def) return false;

    // Проверка условий (уровень, фракция, хроника)
    if (!QuestSys->ValidateQuestPrerequisites(GetOwner(), *Def))
        return false;

    FActiveQuestData NewData;
    NewData.QuestID = QuestID;
    NewData.ObjectiveProgress.SetNum(Def->Objectives.Num());
    // все прогрессы по умолчанию 0
    ActiveQuests.Add(QuestID, NewData);

    // Обновить индекс целей
    QuestSys->RegisterPlayerQuests(GetOwner()->FindComponentByClass<APlayerState>(), this, true);

    return true;
}

void UQuestComponent::UpdateObjective(int32 QuestID, int32 ObjectiveIndex, int32 Delta)
{
    if (!IsServer()) return;

    FActiveQuestData* QuestData = ActiveQuests.Find(QuestID);
    if (!QuestData) return;

    if (!QuestData->ObjectiveProgress.IsValidIndex(ObjectiveIndex))
        return;

    int32& Progress = QuestData->ObjectiveProgress[ObjectiveIndex];
    const FQuestDefinition* Def = GetWorld()->GetSubsystem<UQuestSubsystem>()->GetQuestDefinition(QuestID);
    if (!Def) return;

    const FQuestObjectiveDef& ObjDef = Def->Objectives[ObjectiveIndex];
    Progress += Delta;
    Progress = FMath::Min(Progress, ObjDef.RequiredCount);

    if (CheckQuestCompletion(QuestID))
    {
        CompleteQuest(QuestID);
    }
}

bool UQuestComponent::CheckQuestCompletion(int32 QuestID) const
{
    const FActiveQuestData* QuestData = ActiveQuests.Find(QuestID);
    if (!QuestData) return false;

    const FQuestDefinition* Def = GetWorld()->GetSubsystem<UQuestSubsystem>()->GetQuestDefinition(QuestID);
    if (!Def) return false;

    for (int32 i = 0; i < Def->Objectives.Num(); ++i)
    {
        const FQuestObjectiveDef& ObjDef = Def->Objectives[i];
        const int32 Progress = QuestData->ObjectiveProgress[i];
        if (Progress < ObjDef.RequiredCount)
            return false;
    }
    return true;
}

void UQuestComponent::CompleteQuest(int32 QuestID)
{
    if (!IsServer()) return;

    if (!ActiveQuests.Contains(QuestID))
        return;

    // Удаляем из активных
    ActiveQuests.Remove(QuestID);

    // Добавляем в завершённые
    CompletedQuests.AddUnique(QuestID);

    // Обновить индекс целей (убрать из индекса)
    UQuestSubsystem* QuestSys = GetWorld()->GetSubsystem<UQuestSubsystem>();
    if (QuestSys)
    {
        QuestSys->RegisterPlayerQuests(GetOwner()->FindComponentByClass<APlayerState>(), this, false);
        QuestSys->GrantRewards(GetOwner(), QuestID);
    }
}

void UQuestComponent::FailQuest(int32 QuestID)
{
    if (!IsServer()) return;

    if (!ActiveQuests.Contains(QuestID))
        return;

    ActiveQuests.Remove(QuestID);
    FailedQuests.AddUnique(QuestID);

    UQuestSubsystem* QuestSys = GetWorld()->GetSubsystem<UQuestSubsystem>();
    if (QuestSys)
    {
        QuestSys->RegisterPlayerQuests(GetOwner()->FindComponentByClass<APlayerState>(), this, false);
    }
}
