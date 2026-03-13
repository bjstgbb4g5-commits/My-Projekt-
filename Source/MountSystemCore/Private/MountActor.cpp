#include "MountActor.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Controller.h"
#include "PlayerMountComponent.h"
#include "StableSubsystem.h"
#include "Engine/World.h"

AMountActor::AMountActor()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetReplicateMovement(true);
}

void AMountActor::BeginPlay()
{
    Super::BeginPlay();
}

void AMountActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!HasAuthority()) return;

    UpdateRecentDamage(DeltaTime);
    CheckPanic();
}

void AMountActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMountActor, MountData);
}

void AMountActor::SetMountData(const FMountData& InMountData)
{
    if (!HasAuthority()) return;
    MountData = InMountData;

    // Устанавливаем базовые статы в зависимости от типа (если ещё не заданы)
    // (В реальном проекте можно добавить таблицы)
    ApplyGenesAndMutations();

    MountData.CurrentHealth = MountData.Stats.MaxHealth;
    MountData.CurrentSpeed = MountData.Stats.Speed;
    MountData.bIsAlive = true;
    MountData.RecentDamageTaken = 0.f;
    MountData.bPanicking = false;
}

void AMountActor::ApplyGenesAndMutations()
{
    // Гены модифицируют статы
    MountData.Stats.Speed *= MountData.Genes.SpeedGene;
    MountData.Stats.MaxHealth *= MountData.Genes.StrengthGene;
    MountData.Stats.Armor *= MountData.Genes.StrengthGene;
    MountData.Stats.Stamina *= MountData.Genes.StaminaGene;
    MountData.Stats.CargoCapacity *= MountData.Genes.CargoGene;
}

void AMountActor::TakeDamage(float Damage, AController* Instigator, bool bIsPvE)
{
    if (!HasAuthority()) return;
    if (!MountData.bIsAlive) return;

    // 1. Armor reduction
    float DamageAfterArmor = Damage * (100.f / (100.f + MountData.Stats.Armor));

    // 2. Damage cap (25% max health)
    float MaxDamage = MountData.Stats.MaxHealth * 0.25f;
    DamageAfterArmor = FMath::Min(DamageAfterArmor, MaxDamage);

    // 3. PvE reduction
    if (bIsPvE)
    {
        DamageAfterArmor *= 0.5f;
    }

    // 4. Split damage (60% to player, 40% to mount) – handled outside, здесь просто наносим урон маунту
    // Для простоты будем считать, что урон уже передан как полный, и маунт получает свою долю.
    // На практике вызов должен идти из CombatManager с учётом разделения.

    MountData.CurrentHealth -= DamageAfterArmor;
    if (MountData.CurrentHealth <= 0.f)
    {
        KillMount();
        return;
    }

    RecentDamage.Add({ GetWorld()->GetTimeSeconds(), DamageAfterArmor });
}

void AMountActor::UpdateRecentDamage(float DeltaTime)
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    RecentDamage.RemoveAll([CurrentTime](const FRecentDamageEntry& Entry) { return CurrentTime - Entry.Time > 3.f; });

    float Total = 0.f;
    for (const auto& Entry : RecentDamage)
        Total += Entry.Damage;
    MountData.RecentDamageTaken = Total;
}

void AMountActor::CheckPanic()
{
    if (!MountData.bIsAlive) return;

    float HealthPercent = MountData.CurrentHealth / MountData.Stats.MaxHealth;
    if (HealthPercent < 0.2f && !MountData.bPanicking)
    {
        float PanicChance = 0.5f / FMath::Max(0.1f, MountData.Genes.TemperGene);
        if (FMath::FRand() < PanicChance)
        {
            MountData.bPanicking = true;
            MountData.PanicEndTime = GetWorld()->GetTimeSeconds() + 5.f;
        }
    }

    if (MountData.bPanicking && GetWorld()->GetTimeSeconds() >= MountData.PanicEndTime)
    {
        MountData.bPanicking = false;
    }

    // Knockdown check
    if (MountData.RecentDamageTaken > MountData.Stats.MaxHealth * 0.35f)
    {
        AController* Controller = GetController();
        if (Controller)
        {
            APawn* RiderPawn = Cast<APawn>(GetOwner());
            if (RiderPawn && Controller->GetPawn() == this)
            {
                Controller->Possess(RiderPawn);
            }
        }
    }
}

void AMountActor::Heal(float Amount)
{
    if (!HasAuthority()) return;
    if (!MountData.bIsAlive) return;
    MountData.CurrentHealth = FMath::Min(MountData.CurrentHealth + Amount, MountData.Stats.MaxHealth);
}

void AMountActor::KillMount()
{
    if (!HasAuthority()) return;

    MountData.bIsAlive = false;
    MountData.CurrentHealth = 0.f;

    AController* Controller = GetController();
    if (Controller)
    {
        APawn* RiderPawn = Cast<APawn>(GetOwner());
        if (RiderPawn)
        {
            Controller->Possess(RiderPawn);
        }
        else
        {
            Controller->UnPossess();
        }
    }

    AActor* OwnerActor = GetOwner();
    if (OwnerActor)
    {
        UPlayerMountComponent* MountComp = OwnerActor->FindComponentByClass<UPlayerMountComponent>();
        if (MountComp)
        {
            MountComp->RemoveMount(MountData.MountID);
        }
    }

    Destroy();
}

void AMountActor::RegenHealthInStable(float DeltaTime)
{
    if (!HasAuthority()) return;
    float RegenRate = MountData.Stats.MaxHealth * 0.1f / 60.f; // 10% per minute
    MountData.CurrentHealth = FMath::Min(MountData.CurrentHealth + RegenRate * DeltaTime, MountData.Stats.MaxHealth);
}
