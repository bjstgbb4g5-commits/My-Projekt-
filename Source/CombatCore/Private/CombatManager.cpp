#include "CombatManager.h"
#include "CombatResolver.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "AbilityCastComponent.h"

UCombatManager::UCombatManager()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCombatManager::BeginPlay()
{
    Super::BeginPlay();
}

void UCombatManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (GetOwner()->HasAuthority())
    {
        float CurrentTime = GetWorld()->GetTimeSeconds();
        CleanupFocusFire(CurrentTime);
    }
}

void UCombatManager::CleanupFocusFire(float CurrentTime)
{
    TArray<AActor*> TargetsToRemove;
    for (auto& Pair : FocusFireTracker)
    {
        TArray<FFocusAttack>& Attacks = Pair.Value;
        Attacks.RemoveAll([CurrentTime](const FFocusAttack& Attack)
        {
            return CurrentTime - Attack.LastHitTime > 2.0f;
        });
        if (Attacks.Num() == 0)
        {
            TargetsToRemove.Add(Pair.Key);
        }
    }
    for (AActor* Target : TargetsToRemove)
    {
        FocusFireTracker.Remove(Target);
    }
}

float UCombatManager::CalculateFocusFireBonus(AActor* Target, float CurrentTime, AActor* CurrentAttacker)
{
    TArray<FFocusAttack>* AttacksPtr = FocusFireTracker.Find(Target);
    TSet<AActor*> UniqueAttackers;
    if (AttacksPtr)
    {
        for (const FFocusAttack& Attack : *AttacksPtr)
        {
            if (CurrentTime - Attack.LastHitTime <= 2.0f)
            {
                UniqueAttackers.Add(Attack.Attacker);
            }
        }
    }
    UniqueAttackers.Add(CurrentAttacker);
    return (UniqueAttackers.Num() >= 3) ? 1.15f : 1.0f;
}

void UCombatManager::ApplyAbility(UCharacterEntity* Caster, AActor* TargetActor, const FAbilityInstance& Ability, int32 AbilitySlot)
{
    if (!HasAuthority()) return;
    if (!Caster || !TargetActor) return;

    UCharacterEntity* Target = TargetActor->FindComponentByClass<UCharacterEntity>();
    if (!Target) return;

    if (!Caster->IsAlive() || !Target->IsAlive()) return;

    // Проверка ресурса
    if (!Caster->Resource->HasEnough(Ability.GetResourceCost())) return;

    // Определяем PvP
    bool bIsPvP = Caster->GetOwner()->IsA<APlayerController>() && Target->GetOwner()->IsA<APlayerController>();

    // Фокус-файр бонус
    float CurrentTime = GetWorld()->GetTimeSeconds();
    float FocusBonus = CalculateFocusFireBonus(TargetActor, CurrentTime, Caster->GetOwner());

    // Расчёт урона
    float Damage = CombatResolver::CalculateDamage(Ability, Caster, Target, ElementMatrix, bIsPvP, Config, FocusBonus);

    // Применяем урон
    Target->Server_TakeDamage(Damage);

    // Обновляем FocusFireTracker
    TArray<FFocusAttack>& Attacks = FocusFireTracker.FindOrAdd(TargetActor);
    bool bFound = false;
    for (FFocusAttack& Attack : Attacks)
    {
        if (Attack.Attacker == Caster->GetOwner())
        {
            Attack.LastHitTime = CurrentTime;
            bFound = true;
            break;
        }
    }
    if (!bFound)
    {
        FFocusAttack NewAttack;
        NewAttack.Attacker = Caster->GetOwner();
        NewAttack.LastHitTime = CurrentTime;
        Attacks.Add(NewAttack);
    }

    // Ставим кулдаун
    Caster->AbilityManager->PutOnCooldown(AbilitySlot);
    Caster->Resource->Spend(Ability.GetResourceCost());
}

void UCombatManager::ApplyDamage(UCharacterEntity* Caster, UCharacterEntity* Target, float Damage)
{
    if (!HasAuthority()) return;
    if (!Caster || !Target) return;
    Target->Server_TakeDamage(Damage);
}

void UCombatManager::RequestAttack(UCharacterEntity* Caster, AActor* Target, int32 SlotIndex)
{
    if (!HasAuthority()) return;
    if (!Caster || !Target) return;

    UAbilityCastComponent* CastComp = Caster->GetOwner()->FindComponentByClass<UAbilityCastComponent>();
    if (CastComp)
    {
        CastComp->Server_RequestCast(SlotIndex, Target);
    }
}
