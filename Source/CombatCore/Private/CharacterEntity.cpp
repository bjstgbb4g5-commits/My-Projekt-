#include "CharacterEntity.h"
#include "Net/UnrealNetwork.h"
#include "ResourceSystem/IResourceSystem.h"
#include "ResourceSystem/RageResource.h"
#include "ResourceSystem/ManaResource.h"
#include "ResourceSystem/FocusResource.h"
#include "ResourceSystem/EssenceResource.h"
#include "Engine/World.h"

UCharacterEntity::UCharacterEntity()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);

    IsInCombat = false;
    PreviousHealth = 0.f;
}

void UCharacterEntity::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Owner = GetOwner())
    {
        Owner->bReplicates = true;
    }

    if (HasAuthority())
    {
        CurrentHealth = Stats.Vitality;
        PreviousHealth = CurrentHealth;
        InitializeResource();
    }
}

void UCharacterEntity::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UCharacterEntity, CurrentHealth);
    DOREPLIFETIME(UCharacterEntity, CurrentResource);
}

void UCharacterEntity::InitializeResource()
{
    switch (ResourceType)
    {
    case EResourceType::Rage:
        Resource = NewObject<URageResource>(this);
        Cast<URageResource>(Resource)->Initialize(MaxResource);
        break;
    case EResourceType::Mana:
        Resource = NewObject<UManaResource>(this);
        Cast<UManaResource>(Resource)->Initialize(MaxResource);
        break;
    case EResourceType::Focus:
        Resource = NewObject<UFocusResource>(this);
        Cast<UFocusResource>(Resource)->Initialize(MaxResource);
        break;
    case EResourceType::Essence:
        Resource = NewObject<UEssenceResource>(this);
        Cast<UEssenceResource>(Resource)->Initialize(MaxResource);
        break;
    default:
        break;
    }
}

void UCharacterEntity::OnRep_CurrentHealth()
{
    OnHealthChanged.Broadcast(CurrentHealth);
    if (CurrentHealth <= 0.f && PreviousHealth > 0.f)
    {
        OnDeath.Broadcast();
    }
    PreviousHealth = CurrentHealth;
}

void UCharacterEntity::OnRep_CurrentResource()
{
    // Можно добавить событие
}

void UCharacterEntity::Server_TakeDamage_Implementation(float Damage)
{
    if (!HasAuthority()) return;
    if (!IsAlive()) return;

    CurrentHealth = FMath::Max(0.f, CurrentHealth - Damage);
    if (CurrentHealth <= 0.f)
    {
        HandleDeath();
    }
}

bool UCharacterEntity::Server_TakeDamage_Validate(float Damage)
{
    return Damage >= 0.f && Damage < 100000.f;
}

void UCharacterEntity::Server_ReceiveHeal_Implementation(float Amount)
{
    if (!HasAuthority()) return;
    if (!IsAlive()) return;
    CurrentHealth = FMath::Min(Stats.Vitality, CurrentHealth + Amount);
}

bool UCharacterEntity::Server_ReceiveHeal_Validate(float Amount)
{
    return Amount >= 0.f && Amount < 100000.f;
}

void UCharacterEntity::HandleDeath()
{
    CurrentHealth = 0.f;
    OnDeath.Broadcast();
}
