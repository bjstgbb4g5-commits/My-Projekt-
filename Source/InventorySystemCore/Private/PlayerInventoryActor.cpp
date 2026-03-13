#include "PlayerInventoryActor.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

APlayerInventoryActor::APlayerInventoryActor()
{
    bReplicates = true;
    bAlwaysRelevant = true; // для простоты
}

void APlayerInventoryActor::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        Inventory.InitContainer(40, EInventoryContainerType::PlayerInventory);
        HotbarSlots.SetNum(10); // 10 слотов хотбара
        Equipment.InitContainer(8, EInventoryContainerType::Equipment);
    }
}

void APlayerInventoryActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APlayerInventoryActor, Inventory);
    DOREPLIFETIME(APlayerInventoryActor, Equipment);
    DOREPLIFETIME(APlayerInventoryActor, HotbarSlots);
    DOREPLIFETIME(APlayerInventoryActor, CraftQueue);
}

bool APlayerInventoryActor::IsServer() const
{
    return GetOwner() && GetOwner()->HasAuthority();
}

bool APlayerInventoryActor::IsValidSlot(EInventoryContainerType ContainerType, int32 SlotIndex) const
{
    const FInventoryContainer* Container = nullptr;
    switch (ContainerType)
    {
    case EInventoryContainerType::PlayerInventory: Container = &Inventory; break;
    case EInventoryContainerType::Equipment:       Container = &Equipment; break;
    default: return false;
    }
    return Container && Container->Slots.IsValidIndex(SlotIndex);
}

FInventorySlot* APlayerInventoryActor::GetSlot(EInventoryContainerType ContainerType, int32 SlotIndex)
{
    if (!HasAuthority()) return nullptr;
    if (!IsValidSlot(ContainerType, SlotIndex)) return nullptr;

    switch (ContainerType)
    {
    case EInventoryContainerType::PlayerInventory: return &Inventory.Slots[SlotIndex];
    case EInventoryContainerType::Equipment:       return &Equipment.Slots[SlotIndex];
    default: return nullptr;
    }
}

const FInventorySlot* APlayerInventoryActor::GetSlotConst(EInventoryContainerType ContainerType, int32 SlotIndex) const
{
    if (!IsValidSlot(ContainerType, SlotIndex)) return nullptr;

    switch (ContainerType)
    {
    case EInventoryContainerType::PlayerInventory: return &Inventory.Slots[SlotIndex];
    case EInventoryContainerType::Equipment:       return &Equipment.Slots[SlotIndex];
    default: return nullptr;
    }
}

bool APlayerInventoryActor::SetHotbarSlot(int32 HotbarIndex, int32 InventorySlotIndex)
{
    if (!HasAuthority()) return false;
    if (!HotbarSlots.IsValidIndex(HotbarIndex)) return false;
    if (InventorySlotIndex != -1 && !Inventory.Slots.IsValidIndex(InventorySlotIndex)) return false;

    FHotbarSlot& Slot = HotbarSlots[HotbarIndex];
    Slot.bIsEmpty = (InventorySlotIndex == -1);
    Slot.InventorySlotIndex = InventorySlotIndex;
    return true;
}

int32 APlayerInventoryActor::GetInventorySlotFromHotbar(int32 HotbarIndex) const
{
    if (!HotbarSlots.IsValidIndex(HotbarIndex)) return -1;
    return HotbarSlots[HotbarIndex].InventorySlotIndex;
}

int32 APlayerInventoryActor::GenerateNextQueueID()
{
    return NextQueueID++;
}

void APlayerInventoryActor::AddToCraftQueue(int32 QueueID, int32 RecipeID, float StartTime, float Duration)
{
    if (!HasAuthority()) return;
    FCraftQueueItem Item;
    Item.QueueID = QueueID;
    Item.RecipeID = RecipeID;
    Item.StartTime = StartTime;
    Item.CraftDuration = Duration;
    CraftQueue.Add(Item);
}

void APlayerInventoryActor::RemoveFromCraftQueue(int32 QueueID)
{
    if (!HasAuthority()) return;
    CraftQueue.RemoveAll([QueueID](const FCraftQueueItem& Item) { return Item.QueueID == QueueID; });
}
