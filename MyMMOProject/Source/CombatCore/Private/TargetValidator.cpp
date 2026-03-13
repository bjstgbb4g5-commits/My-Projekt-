#include "TargetValidator.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "CharacterEntity.h"

bool UTargetValidator::IsTargetValid(AActor* Caster, AActor* Target, float Range) const
{
    if (!Caster || !Target) return false;

    // Проверка живости
    UCharacterEntity* TargetEntity = Target->FindComponentByClass<UCharacterEntity>();
    if (TargetEntity && !TargetEntity->IsAlive()) return false;

    // Дистанция
    float Dist = FVector::Dist(Caster->GetActorLocation(), Target->GetActorLocation());
    if (Dist > Range) return false;

    // LOS
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Caster);
    Params.AddIgnoredActor(Target);
    if (GetWorld()->LineTraceSingleByChannel(Hit, Caster->GetActorLocation(), Target->GetActorLocation(), ECC_Visibility, Params))
    {
        if (Hit.GetActor() != Target) return false;
    }

    return true;
}

bool UTargetValidator::IsTargetValidWithAbility(AActor* Caster, AActor* Target, UAbilityDataAsset* Ability) const
{
    return IsTargetValid(Caster, Target, Ability ? Ability->Range : 0.f);
}
