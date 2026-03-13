#include "ItemSubsystem.h"
#include "Engine/DataTable.h"

UItemSubsystem::UItemSubsystem()
{
}

void UItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    LoadItems();
}

void UItemSubsystem::Deinitialize()
{
    ItemDefinitions.Empty();
    Super::Deinitialize();
}

void UItemSubsystem::LoadItems()
{
    // Загружаем DataTable с определениями предметов
    UDataTable* Table = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Items.Items"));
    if (!Table)
    {
        UE_LOG(LogTemp, Error, TEXT("UItemSubsystem: Item table not found at /Game/Data/Items.Items"));
        return;
    }

    TArray<FItemDefinition*> Rows;
    Table->GetAllRows(TEXT("Items"), Rows);
    for (FItemDefinition* Row : Rows)
    {
        if (Row->ItemID <= 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("UItemSubsystem: Item with invalid ID skipped"));
            continue;
        }
        ItemDefinitions.Add(Row->ItemID, *Row);
    }
    UE_LOG(LogTemp, Log, TEXT("UItemSubsystem: Loaded %d item definitions."), ItemDefinitions.Num());
}

const FItemDefinition* UItemSubsystem::GetItemDefinition(int32 ItemID) const
{
    return ItemDefinitions.Find(ItemID);
}

FItemInstance UItemSubsystem::CreateItem(int32 ItemID, int32 Quantity)
{
    FItemInstance Result;

    // Проверка корректности количества
    if (Quantity <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("UItemSubsystem::CreateItem: invalid quantity %d for ItemID %d"), Quantity, ItemID);
        return Result;
    }

    const FItemDefinition* Def = GetItemDefinition(ItemID);
    if (!Def)
    {
        UE_LOG(LogTemp, Warning, TEXT("UItemSubsystem::CreateItem: ItemID %d not found"), ItemID);
        return Result;
    }

    Result.ItemID = ItemID;
    Result.Quantity = Quantity;
    // Учитываем, стакается ли предмет
    Result.MaxStack = Def->bStackable ? Def->MaxStack : 1;
    Result.Durability = Def->DurabilityMax;
    Result.Rarity = Def->Rarity;

    return Result;
}
