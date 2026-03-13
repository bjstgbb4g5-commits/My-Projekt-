#include "InventorySubsystem.h"
#include "PlayerInventoryActor.h"
#include "GameFramework/Pawn.h"

UInventorySubsystem::UInventorySubsystem()
{
}

bool UInventorySubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

// Вспомогательная функция для подсчёта предметов в одном контейнере
int32 UInventorySubsystem::CountItemInContainer(const FInventoryContainer& Container, int32 ItemID) const
{
    int32 Total = 0;
    for (const FInventorySlot& Slot : Container.Slots)
    {
        if (Slot.bOccupied && Slot.Item.ItemID == ItemID)
            Total += Slot.Item.Quantity;
    }
    return Total;
}

// Вспомогательная функция для удаления предмета из контейнера (возвращает true, если удаление завершено)
bool UInventorySubsystem::RemoveItemByIDFromContainer(FInventoryContainer& Container, int32 ItemID, int32& Amount)
{
    int32 Remaining = Amount;
    for (FInventorySlot& Slot : Container.Slots)
    {
        if (!Slot.bOccupied || Slot.Item.ItemID != ItemID) continue;
        int32 RemoveNow = FMath::Min(Slot.Item.Quantity, Remaining);
        Slot.Item.Quantity -= RemoveNow;
        Remaining -= RemoveNow;
        if (Slot.Item.Quantity <= 0)
        {
            Slot.bOccupied = false;
            Slot.Item = FItemInstance();
        }
        if (Remaining <= 0) break;
    }
    Amount -= Remaining; // Amount становится количеством удалённых
    return Remaining == 0;
}

bool UInventorySubsystem::AddItem(APlayerInventoryActor* InventoryActor, EInventoryContainerType ContainerType, const FItemInstance& Item)
{
    if (!IsServer() || !InventoryActor) return false;

    FInventoryContainer* Container = nullptr;
    switch (ContainerType)
    {
    case EInventoryContainerType::PlayerInventory: Container = &InventoryActor->Inventory; break;
    case EInventoryContainerType::Equipment:       Container = &InventoryActor->Equipment; break;
    default: return false;
    }

    int32 Remaining = Item.Quantity;
    int32 MaxStack = Item.MaxStack;

    // 1. Проверка возможности размещения (атомарно)
    int32 SpaceInExisting = 0;
    for (const FInventorySlot& Slot : Container->Slots)
    {
        if (Slot.bOccupied && Slot.Item.ItemID == Item.ItemID && Slot.Item.Quantity < MaxStack)
        {
            SpaceInExisting += (MaxStack - Slot.Item.Quantity);
        }
    }
    int32 AfterExisting = FMath::Max(0, Remaining - SpaceInExisting);
    int32 SlotsNeeded = FMath::DivideAndRoundUp(AfterExisting, MaxStack);

    int32 FreeSlots = 0;
    for (const FInventorySlot& Slot : Container->Slots)
    {
        if (!Slot.bOccupied) FreeSlots++;
    }
    if (SlotsNeeded > FreeSlots) return false;

    // 2. Безопасное добавление
    Remaining = Item.Quantity;

    // Заполняем существующие неполные стопки
    for (FInventorySlot& Slot : Container->Slots)
    {
        if (Slot.bOccupied && Slot.Item.ItemID == Item.ItemID && Slot.Item.Quantity < MaxStack)
        {
            int32 CanAdd = MaxStack - Slot.Item.Quantity;
            int32 AddNow = FMath::Min(Remaining, CanAdd);
            Slot.Item.Quantity += AddNow;
            Remaining -= AddNow;
            if (Remaining <= 0) return true;
        }
    }

    // Создаём новые стопки
    for (FInventorySlot& Slot : Container->Slots)
    {
        if (!Slot.bOccupied)
        {
            int32 ToAdd = FMath::Min(Remaining, MaxStack);
            Slot.bOccupied = true;
            Slot.Item = Item;
            Slot.Item.Quantity = ToAdd;
            Remaining -= ToAdd;
            if (Remaining <= 0) return true;
        }
    }

    return false; // не должно дойти
}

bool UInventorySubsystem::RemoveItem(APlayerInventoryActor* InventoryActor, EInventoryContainerType ContainerType, int32 SlotIndex)
{
    if (!IsServer() || !InventoryActor) return false;

    FInventorySlot* Slot = InventoryActor->GetSlot(ContainerType, SlotIndex);
    if (!Slot || !Slot->bOccupied) return false;

    Slot->bOccupied = false;
    Slot->Item = FItemInstance();
    return true;
}

bool UInventorySubsystem::MoveItem(APlayerInventoryActor* InventoryActor, EInventoryContainerType ContainerType, int32 FromSlot, int32 ToSlot)
{
    if (!IsServer() || !InventoryActor) return false;
    if (FromSlot == ToSlot) return false;

    FInventorySlot* SlotFrom = InventoryActor->GetSlot(ContainerType, FromSlot);
    FInventorySlot* SlotTo   = InventoryActor->GetSlot(ContainerType, ToSlot);
    if (!SlotFrom || !SlotTo) return false;
    if (!SlotFrom->bOccupied) return false;

    if (SlotTo->bOccupied)
    {
        // Обмен
        Swap(*SlotFrom, *SlotTo);
    }
    else
    {
        SlotTo->bOccupied = true;
        SlotTo->Item = SlotFrom->Item;
        SlotFrom->bOccupied = false;
        SlotFrom->Item = FItemInstance();
    }
    return true;
}

bool UInventorySubsystem::EquipItem(APlayerInventoryActor* InventoryActor, int32 InventorySlot, int32 EquipmentSlot)
{
    if (!IsServer() || !InventoryActor) return false;

    FInventorySlot* InvSlot = InventoryActor->GetSlot(EInventoryContainerType::PlayerInventory, InventorySlot);
    FInventorySlot* EquipSlot = InventoryActor->GetSlot(EInventoryContainerType::Equipment, EquipmentSlot);
    if (!InvSlot || !EquipSlot) return false;
    if (!InvSlot->bOccupied) return false;

    // Здесь можно добавить проверку, что предмет можно экипировать (по типу)

    if (EquipSlot->bOccupied)
    {
        Swap(*InvSlot, *EquipSlot);
    }
    else
    {
        EquipSlot->bOccupied = true;
        EquipSlot->Item = InvSlot->Item;
        InvSlot->bOccupied = false;
        InvSlot->Item = FItemInstance();
    }
    return true;
}

bool UInventorySubsystem::UnequipItem(APlayerInventoryActor* InventoryActor, int32 EquipmentSlot, int32 InventorySlot)
{
    return EquipItem(InventoryActor, InventorySlot, EquipmentSlot); // обратная операция
}

int32 UInventorySubsystem::CountItem(APlayerInventoryActor* InventoryActor, int32 ItemID) const
{
    if (!InventoryActor) return 0;
    int32 Total = 0;
    Total += CountItemInContainer(InventoryActor->Inventory, ItemID);
    Total += CountItemInContainer(InventoryActor->Equipment, ItemID);
    return Total;
}

bool UInventorySubsystem::RemoveItemByID(APlayerInventoryActor* InventoryActor, int32 ItemID, int32 Amount)
{
    if (!IsServer() || !InventoryActor || Amount <= 0) return false;

    int32 Remaining = Amount;

    // Сначала из инвентаря
    if (!RemoveItemByIDFromContainer(InventoryActor->Inventory, ItemID, Remaining) && Remaining > 0)
    {
        // Если не хватило в инвентаре, идём в экипировку
        if (!RemoveItemByIDFromContainer(InventoryActor->Equipment, ItemID, Remaining))
            return false;
    }
    return (Remaining == 0);
}
