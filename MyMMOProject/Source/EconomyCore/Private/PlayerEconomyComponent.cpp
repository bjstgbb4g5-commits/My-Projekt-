#include "PlayerEconomyComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "PoliticalIdentityComponent.h"
#include "CityEconomyComponent.h"
#include "Engine/World.h"

UPlayerEconomyComponent::UPlayerEconomyComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);

    Gold = 0;
    TradeReputation = 0;
}

void UPlayerEconomyComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UPlayerEconomyComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UPlayerEconomyComponent, Gold);
    DOREPLIFETIME(UPlayerEconomyComponent, Resources);
    DOREPLIFETIME(UPlayerEconomyComponent, Items);
    DOREPLIFETIME(UPlayerEconomyComponent, TradeReputation);
}

bool UPlayerEconomyComponent::IsServer() const
{
    return GetOwner() && GetOwner()->HasAuthority();
}

UCityEconomyComponent* UPlayerEconomyComponent::GetCityEconomy() const
{
    AActor* Owner = GetOwner();
    if (!Owner) return nullptr;
    UPoliticalIdentityComponent* Identity = Owner->FindComponentByClass<UPoliticalIdentityComponent>();
    if (!Identity) return nullptr;
    // Здесь нужен реестр городов, пока заглушка
    return nullptr;
}

void UPlayerEconomyComponent::AddGold(int32 Amount)
{
    if (!IsServer()) return;
    Gold += Amount;
}

bool UPlayerEconomyComponent::RemoveGold(int32 Amount)
{
    if (!IsServer()) return false;
    if (Gold < Amount) return false;
    Gold -= Amount;
    return true;
}

void UPlayerEconomyComponent::AddResource(EResourceType Type, int32 Amount)
{
    if (!IsServer()) return;
    Resources.FindOrAdd(Type) += Amount;
}

bool UPlayerEconomyComponent::RemoveResource(EResourceType Type, int32 Amount)
{
    if (!IsServer()) return false;
    int32& Current = Resources.FindOrAdd(Type);
    if (Current < Amount) return false;
    Current -= Amount;
    return true;
}

int32 UPlayerEconomyComponent::GetResourceAmount(EResourceType Type) const
{
    const int32* Found = Resources.Find(Type);
    return Found ? *Found : 0;
}

void UPlayerEconomyComponent::AddItem(int32 ItemID, int32 Quantity, bool bStolen)
{
    if (!IsServer()) return;
    for (FItemStack& Stack : Items)
    {
        if (Stack.ItemID == ItemID && Stack.bStolen == bStolen)
        {
            Stack.Quantity += Quantity;
            return;
        }
    }
    FItemStack NewStack;
    NewStack.ItemID = ItemID;
    NewStack.Quantity = Quantity;
    NewStack.bStolen = bStolen;
    Items.Add(NewStack);
}

bool UPlayerEconomyComponent::RemoveItem(int32 ItemID, int32 Quantity)
{
    if (!IsServer()) return false;
    for (int32 i = 0; i < Items.Num(); ++i)
    {
        if (Items[i].ItemID == ItemID)
        {
            if (Items[i].Quantity >= Quantity)
            {
                Items[i].Quantity -= Quantity;
                if (Items[i].Quantity <= 0)
                {
                    Items.RemoveAt(i);
                }
                return true;
            }
            return false;
        }
    }
    return false;
}

bool UPlayerEconomyComponent::HasItem(int32 ItemID, int32 Quantity) const
{
    for (const FItemStack& Stack : Items)
    {
        if (Stack.ItemID == ItemID && Stack.Quantity >= Quantity)
            return true;
    }
    return false;
}

void UPlayerEconomyComponent::NotifyIncome(int32 Amount)
{
    if (!IsServer()) return;
    // Найти город и заплатить налог
    UCityEconomyComponent* CityEcon = GetCityEconomy();
    if (CityEcon)
    {
        CityEcon->CollectTax(Amount);
        // Остаток уже будет добавлен через AddGold вызывающим кодом
    }
    else
    {
        AddGold(Amount);
    }
}
