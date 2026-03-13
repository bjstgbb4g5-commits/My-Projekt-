#include "QuestManager.h"
#include "QuestSubsystem.h"

void UQuestManager::Initialize(UQuestSubsystem* InSubsystem)
{
    QuestSubsystem = InSubsystem;
    RegisterDelegates();
}

void UQuestManager::RegisterDelegates()
{
    // Здесь нужно подписаться на глобальные события из CombatCore, EconomyCore и т.д.
    // Например:
    // UCombatEventSubsystem* CombatEvents = GetWorld()->GetSubsystem<UCombatEventSubsystem>();
    // CombatEvents->OnEnemyKilled.AddDynamic(this, &UQuestManager::OnEnemyKilled);
}

void UQuestManager::UnregisterDelegates()
{
    // Отписаться
}

void UQuestManager::OnEnemyKilled(AActor* Killer, AActor* Victim, int32 EnemyID)
{
    if (QuestSubsystem)
    {
        QuestSubsystem->OnEnemyKilled(Killer, Victim, EnemyID);
    }
}

void UQuestManager::OnItemCollected(APlayerState* Player, int32 ItemID, int32 Quantity)
{
    if (QuestSubsystem)
    {
        QuestSubsystem->OnItemCollected(Player, ItemID, Quantity);
    }
}

void UQuestManager::OnLocationVisited(APlayerState* Player, int32 RegionID)
{
    if (QuestSubsystem)
    {
        QuestSubsystem->OnLocationVisited(Player, RegionID);
    }
}

void UQuestManager::OnNPCInteracted(APlayerState* Player, int32 NPCID)
{
    if (QuestSubsystem)
    {
        QuestSubsystem->OnNPCInteracted(Player, NPCID);
    }
}

void UQuestManager::OnCraftCompleted(APlayerState* Player, int32 ItemID)
{
    if (QuestSubsystem)
    {
        QuestSubsystem->OnCraftCompleted(Player, ItemID);
    }
}

void UQuestManager::OnWorldEventParticipated(APlayerState* Player, int32 EventID)
{
    if (QuestSubsystem)
    {
        QuestSubsystem->OnWorldEventParticipated(Player, EventID);
    }
}
