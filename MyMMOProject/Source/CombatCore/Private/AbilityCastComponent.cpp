#include "AbilityCastComponent.h"
#include "CharacterEntity.h"
#include "GameConfig.h"
#include "CombatManager.h"
#include "Engine/World.h"
#include "TimerManager.h"

UAbilityCastComponent::UAbilityCastComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);

    bIsCasting = false;
    bIsGCDActive = false;
    CurrentCastingSlot = -1;
}

void UAbilityCastComponent::BeginPlay()
{
    Super::BeginPlay();
    if (AActor* Owner = GetOwner())
    {
        OwnerEntity = Owner->FindComponentByClass<UCharacterEntity>();
        if (OwnerEntity)
        {
            OwnerEntity->OnHealthChanged.AddDynamic(this, &UAbilityCastComponent::OnOwnerDamaged);
        }
    }
}

void UAbilityCastComponent::Initialize(UCharacterEntity* InCharacter, UGameConfig* InConfig)
{
    OwnerEntity = InCharacter;
    Config = InConfig;
}

void UAbilityCastComponent::OnOwnerDamaged(float DamagePercent)
{
    if (bIsCasting && DamagePercent >= Config->InterruptDamageThreshold)
    {
        InterruptCast(TEXT("damage threshold exceeded"));
    }
}

bool UAbilityCastComponent::CanCast(const FAbilityInstance& Ability, AActor* Target) const
{
    if (!OwnerEntity || !OwnerEntity->IsAlive()) return false;
    if (bIsCasting) return false;
    if (bIsGCDActive) return false;
    if (!OwnerEntity->Resource->HasEnough(Ability.GetResourceCost())) return false;
    // Проверка дальности и LOS будет в CombatManager
    return true;
}

void UAbilityCastComponent::StartCast(const FAbilityInstance& Ability, AActor* Target, int32 SlotIndex)
{
    if (!CanCast(Ability, Target)) return;

    bIsCasting = true;
    CurrentCastingSlot = SlotIndex;
    CurrentTarget = Target;

    OnCastStarted.Broadcast();

    float CastTime = Ability.BaseData->CastTime;
    if (CastTime > 0.f)
    {
        GetWorld()->GetTimerManager().SetTimer(CastTimerHandle, this, &UAbilityCastComponent::CompleteCast, CastTime, false);
    }
    else
    {
        CompleteCast();
    }
}

void UAbilityCastComponent::CompleteCast()
{
    bIsCasting = false;

    if (!CurrentTarget.IsValid())
    {
        InterruptCast(TEXT("target lost"));
        return;
    }

    // Применяем способность через CombatManager
    UCombatManager* CombatManager = GetWorld()->GetAuthGameMode()->FindComponentByClass<UCombatManager>();
    if (CombatManager)
    {
        FAbilityInstance Ability = OwnerEntity->AbilityManager->GetAbility(CurrentCastingSlot);
        CombatManager->ApplyAbility(OwnerEntity, CurrentTarget.Get(), Ability, CurrentCastingSlot);
    }

    StartGCD();
    OnCastCompleted.Broadcast();
}

void UAbilityCastComponent::StartGCD()
{
    bIsGCDActive = true;
    float Haste = OwnerEntity ? OwnerEntity->Stats.Haste : 0.f;
    float GCDTime = Config->BaseGCD / (1.f + Haste);
    GCDTime = FMath::Max(Config->MinGCD, GCDTime);
    GetWorld()->GetTimerManager().SetTimer(GCDTimerHandle, [this]()
    {
        bIsGCDActive = false;
    }, GCDTime, false);
}

void UAbilityCastComponent::InterruptCast(const FString& Reason)
{
    bIsCasting = false;
    GetWorld()->GetTimerManager().ClearTimer(CastTimerHandle);
    OnCastInterrupted.Broadcast();
    UE_LOG(LogTemp, Verbose, TEXT("Cast interrupted: %s"), *Reason);
}

void UAbilityCastComponent::Server_RequestCast_Implementation(int32 AbilitySlot, AActor* Target)
{
    if (!HasAuthority()) return;
    if (!OwnerEntity) return;

    FAbilityInstance Ability = OwnerEntity->AbilityManager->GetAbility(AbilitySlot);
    if (!Ability.BaseData) return;

    if (CanCast(Ability, Target))
    {
        StartCast(Ability, Target, AbilitySlot);
    }
}

bool UAbilityCastComponent::Server_RequestCast_Validate(int32 AbilitySlot, AActor* Target)
{
    return AbilitySlot >= 0 && Target != nullptr;
}
