#include "QuestSubsystem.h"
#include "QuestComponent.h"
#include "QuestManager.h"
#include "Engine/DataTable.h"
#include "GameFramework/PlayerState.h"
#include "PlayerRegistrySubsystem.h"
#include "EconomyCore/Public/PlayerEconomyComponent.h"
#include "CharacterProgressionCore/Public/PlayerProgressionComponent.h"
#include "ChronicleSystemCore/Public/ChronicleSubsystem.h"
#include "Engine/World.h"

UQuestSubsystem::UQuestSubsystem()
{
}

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    LoadQuestDefinitions();
    QuestManager = NewObject<UQuestManager>(this);
    QuestManager->Initialize(this);
}

void UQuestSubsystem::Deinitialize()
{
    QuestDefinitions.Empty();
    TargetIndex.Empty();
    PlayerQuestComponents.Empty();

    if (QuestManager)
    {
        QuestManager->UnregisterDelegates();
    }

    Super::Deinitialize();
}

void UQuestSubsystem::LoadQuestDefinitions()
{
    QuestTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Quests.Quests"));
    if (!QuestTable)
    {
        UE_LOG(LogTemp, Error, TEXT("UQuestSubsystem: Quest table not found"));
        return;
    }

    TArray<FQuestDefinition*> Rows;
    QuestTable->GetAllRows(TEXT("Quests"), Rows);
    for (FQuestDefinition* Row : Rows)
    {
        if (Row->QuestID > 0)
        {
            QuestDefinitions.Add(Row->QuestID, *Row);
        }
    }
    UE_LOG(LogTemp, Log, TEXT("UQuestSubsystem: Loaded %d quests."), QuestDefinitions.Num());
}

const FQuestDefinition* UQuestSubsystem::GetQuestDefinition(int32 QuestID) const
{
    return QuestDefinitions.Find(QuestID);
}

FQuestTargetKey UQuestSubsystem::MakeKey(EQuestObjectiveType Type, int32 TargetID) const
{
    FQuestTargetKey Key;
    Key.Type = Type;
    Key.TargetID = TargetID;
    return Key;
}

void UQuestSubsystem::RegisterPlayerQuests(APlayerState* PlayerState, UQuestComponent* QuestComp, bool bAdd)
{
    if (!PlayerState || !QuestComp) return;
    FUniqueNetIdRepl PlayerId = PlayerState->GetUniqueId();
    if (!PlayerId.IsValid()) return;

    if (bAdd)
    {
        PlayerQuestComponents.Add(PlayerId, QuestComp);
    }

    // Обновляем индекс для всех активных квестов игрока
    for (const auto& Pair : QuestComp->ActiveQuests)
    {
        int32 QuestID = Pair.Key;
        const FQuestDefinition* Def = GetQuestDefinition(QuestID);
        if (!Def) continue;

        for (int32 ObjIdx = 0; ObjIdx < Def->Objectives.Num(); ++ObjIdx)
        {
            const FQuestObjectiveDef& ObjDef = Def->Objectives[ObjIdx];
            FQuestTargetKey Key = MakeKey(ObjDef.ObjectiveType, ObjDef.TargetID);

            FQuestTargetIndexEntry Entry;
            Entry.PlayerId = PlayerId;
            Entry.QuestID = QuestID;
            Entry.ObjectiveIndex = ObjIdx;

            if (bAdd)
            {
                TargetIndex.FindOrAdd(Key).Add(Entry);
            }
            else
            {
                TArray<FQuestTargetIndexEntry>* Entries = TargetIndex.Find(Key);
                if (Entries)
                {
                    Entries->RemoveAll([&](const FQuestTargetIndexEntry& E) { return E.PlayerId == PlayerId && E.QuestID == QuestID && E.ObjectiveIndex == ObjIdx; });
                    if (Entries->Num() == 0)
                        TargetIndex.Remove(Key);
                }
            }
        }
    }
}

void UQuestSubsystem::UnregisterPlayer(APlayerState* PlayerState)
{
    if (!PlayerState) return;
    FUniqueNetIdRepl PlayerId = PlayerState->GetUniqueId();
    if (!PlayerId.IsValid()) return;

    // Удаляем все записи игрока из индекса
    TArray<FQuestTargetKey> KeysToRemove;
    for (auto& Pair : TargetIndex)
    {
        Pair.Value.RemoveAll([&](const FQuestTargetIndexEntry& E) { return E.PlayerId == PlayerId; });
        if (Pair.Value.Num() == 0)
            KeysToRemove.Add(Pair.Key);
    }
    for (const FQuestTargetKey& Key : KeysToRemove)
    {
        TargetIndex.Remove(Key);
    }

    PlayerQuestComponents.Remove(PlayerId);
}

void UQuestSubsystem::NotifyObjectiveProgress(AActor* TargetActor, EQuestObjectiveType Type, int32 TargetID, int32 Delta)
{
    FQuestTargetKey Key = MakeKey(Type, TargetID);
    const TArray<FQuestTargetIndexEntry>* Entries = TargetIndex.Find(Key);
    if (!Entries) return;

    UPlayerRegistrySubsystem* Registry = GetWorld()->GetSubsystem<UPlayerRegistrySubsystem>();
    if (!Registry) return;

    for (const FQuestTargetIndexEntry& Entry : *Entries)
    {
        APlayerController* PC = Registry->GetPlayerById(Entry.PlayerId);
        if (!PC || !PC->GetPawn()) continue;

        UQuestComponent* QuestComp = PC->GetPawn()->FindComponentByClass<UQuestComponent>();
        if (QuestComp)
        {
            QuestComp->UpdateObjective(Entry.QuestID, Entry.ObjectiveIndex, Delta);
        }
    }
}

bool UQuestSubsystem::ValidateQuestPrerequisites(AActor* PlayerActor, const FQuestDefinition& Def) const
{
    // Проверка уровня (пример)
    UPlayerProgressionComponent* Progression = PlayerActor->FindComponentByClass<UPlayerProgressionComponent>();
    if (Progression && Progression->GetLevel() < Def.RequiredLevel)
        return false;

    // Проверка хроники
    UChronicleSubsystem* Chronicle = GetWorld()->GetSubsystem<UChronicleSubsystem>();
    if (Chronicle && Chronicle->GetCurrentPhase() < Def.ChroniclePhase)
        return false;

    // Проверка фракции и репутации можно добавить позже

    return true;
}

void UQuestSubsystem::GrantRewards(AActor* PlayerActor, int32 QuestID)
{
    if (!PlayerActor || !PlayerActor->HasAuthority()) return;

    const FQuestDefinition* Def = GetQuestDefinition(QuestID);
    if (!Def) return;

    APawn* Pawn = Cast<APawn>(PlayerActor);
    if (!Pawn) return;

    UPlayerEconomyComponent* Economy = Pawn->FindComponentByClass<UPlayerEconomyComponent>();
    if (Economy)
    {
        Economy->AddGold(Def->Rewards.Gold);
        for (const auto& Item : Def->Rewards.ItemRewards)
        {
            // Добавить предмет через ItemSystem
        }
    }

    UPlayerProgressionComponent* Progression = Pawn->FindComponentByClass<UPlayerProgressionComponent>();
    if (Progression)
    {
        // Progression->AddExperience(Def->Rewards.Experience);
    }
}

// Обработчики событий (вызываются из QuestManager)
void UQuestSubsystem::OnEnemyKilled(AActor* Killer, AActor* Victim, int32 EnemyID)
{
    NotifyObjectiveProgress(Killer, EQuestObjectiveType::KillTarget, EnemyID, 1);
}

void UQuestSubsystem::OnItemCollected(APlayerState* Player, int32 ItemID, int32 Quantity)
{
    if (Player && Player->GetPawn())
    {
        NotifyObjectiveProgress(Player->GetPawn(), EQuestObjectiveType::CollectItem, ItemID, Quantity);
    }
}

void UQuestSubsystem::OnLocationVisited(APlayerState* Player, int32 RegionID)
{
    if (Player && Player->GetPawn())
    {
        NotifyObjectiveProgress(Player->GetPawn(), EQuestObjectiveType::VisitLocation, RegionID, 1);
    }
}

void UQuestSubsystem::OnNPCInteracted(APlayerState* Player, int32 NPCID)
{
    if (Player && Player->GetPawn())
    {
        NotifyObjectiveProgress(Player->GetPawn(), EQuestObjectiveType::InteractNPC, NPCID, 1);
    }
}

void UQuestSubsystem::OnCraftCompleted(APlayerState* Player, int32 ItemID)
{
    if (Player && Player->GetPawn())
    {
        NotifyObjectiveProgress(Player->GetPawn(), EQuestObjectiveType::CraftItem, ItemID, 1);
    }
}

void UQuestSubsystem::OnWorldEventParticipated(APlayerState* Player, int32 EventID)
{
    if (Player && Player->GetPawn())
    {
        NotifyObjectiveProgress(Player->GetPawn(), EQuestObjectiveType::WorldEventParticipation, EventID, 1);
    }
}
