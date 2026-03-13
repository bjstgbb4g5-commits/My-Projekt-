#include "LootSubsystem.h"
#include "LootActor.h"
#include "Engine/World.h"
#include "InventorySubsystem.h"
#include "AIDirectorCore/Public/AIWorldDirectorSubsystem.h"
#include "ItemSystemCore/Public/ItemSubsystem.h"
#include "Engine/DataTable.h"
#include "Math/UnrealMathUtility.h"

ULootSubsystem::ULootSubsystem()
{
}

void ULootSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        LoadLootTables();
        AIDirector = GetWorld()->GetSubsystem<UAIDirectorSubsystem>();
        ItemSystem = GetWorld()->GetSubsystem<UItemSubsystem>();
    }
}

void ULootSubsystem::Deinitialize()
{
    LootTables.Empty();
    ActiveLoot.Empty();
    Super::Deinitialize();
}

bool ULootSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

void ULootSubsystem::LoadLootTables()
{
    UDataTable* LootTableData = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/LootTables.LootTables"));
    if (LootTableData)
    {
        TArray<FLootTable*> Rows;
        LootTableData->GetAllRows(TEXT("Loot"), Rows);
        for (FLootTable* Row : Rows)
        {
            LootTables.Add(Row->TableID, *Row);
        }
        UE_LOG(LogTemp, Log, TEXT("ULootSubsystem: Loaded %d loot tables."), LootTables.Num());
    }
    else
    {
        // Заглушка для тестирования
        FLootTable TestTable;
        TestTable.TableID = 1;
        TestTable.Entries.Add({1001, 0.7f, 1, 1}); // Meat
        TestTable.Entries.Add({1002, 0.4f, 1, 1}); // Hide
        LootTables.Add(1, TestTable);
    }
}

TArray<FItemInstance> ULootSubsystem::GenerateLoot(int32 LootTableID, ELootSourceType SourceType)
{
    TArray<FItemInstance> Result;
    const FLootTable* Table = LootTables.Find(LootTableID);
    if (!Table) return Result;

    float Multiplier = 1.0f;
    if (AIDirector)
    {
        Multiplier = AIDirector->GetLootMultiplier(SourceType); // предполагаем такой метод
    }

    for (const FLootEntry& Entry : Table->Entries)
    {
        float AdjustedChance = FMath::Clamp(Entry.DropChance * Multiplier, 0.0f, 1.0f);
        if (AdjustedChance <= 0.0f) continue;

        float Roll = FMath::FRand();
        if (Roll <= AdjustedChance)
        {
            int32 Quantity = FMath::RandRange(Entry.MinQuantity, Entry.MaxQuantity);
            FItemInstance Item;
            if (ItemSystem)
            {
                Item = ItemSystem->CreateItem(Entry.ItemID, Quantity);
            }
            else
            {
                Item.ItemID = Entry.ItemID;
                Item.Quantity = Quantity;
                Item.MaxStack = 999;
            }
            Result.Add(Item);
        }
    }
    return Result;
}

void ULootSubsystem::SpawnLoot(const FVector& Location, const TArray<FItemInstance>& Items)
{
    if (!IsServer()) return;

    CleanupLootArray();
    if (ActiveLoot.Num() >= MAX_LOOT_ACTORS)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot spawn loot: max loot actors reached (%d)"), MAX_LOOT_ACTORS);
        return;
    }

    UWorld* World = GetWorld();
    for (const FItemInstance& Item : Items)
    {
        FVector Offset(
            FMath::RandRange(-50.f, 50.f),
            FMath::RandRange(-50.f, 50.f),
            20.f
        );
        FActorSpawnParameters SpawnParams;
        ALootActor* Loot = World->SpawnActor<ALootActor>(ALootActor::StaticClass(), Location + Offset, FRotator::ZeroRotator, SpawnParams);
        if (Loot)
        {
            Loot->Item = Item;
            ActiveLoot.Add(Loot);
        }
    }
}

bool ULootSubsystem::PickupLoot(ALootActor* Loot, APlayerInventoryActor* Inventory)
{
    if (!IsServer() || !Loot || !Inventory) return false;

    UInventorySubsystem* InventorySys = GetWorld()->GetSubsystem<UInventorySubsystem>();
    if (!InventorySys) return false;

    bool Added = InventorySys->AddItem(Inventory, EInventoryContainerType::PlayerInventory, Loot->Item);
    if (Added)
    {
        ActiveLoot.Remove(Loot);
        Loot->Destroy();
    }
    return Added;
}

void ULootSubsystem::CleanupLootArray()
{
    ActiveLoot.RemoveAll([](const TWeakObjectPtr<ALootActor>& Ptr) { return !Ptr.IsValid(); });
}
