#include "NoxBossCharacter.h"
#include "NoxEventSubsystem.h"
#include "Engine/World.h"

ANoxBossCharacter::ANoxBossCharacter()
{
}

void ANoxBossCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (HasAuthority())
    {
        EventSubsystem = GetWorld()->GetSubsystem<UNoxEventSubsystem>();
    }
}

void ANoxBossCharacter::InitializeHealth(float Health)
{
    if (HasAuthority())
    {
        CurrentHealth = Health;
        MaxHealth = Health;
    }
}

float ANoxBossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    if (HasAuthority())
    {
        float HealthPercent = CurrentHealth / MaxHealth;
        UpdatePhase(HealthPercent);

        if (CurrentHealth <= 0.f)
        {
            if (EventSubsystem)
            {
                EventSubsystem->CompleteNoxEvent();
            }
            Destroy();
        }
    }
    return ActualDamage;
}

void ANoxBossCharacter::UpdatePhase(float HealthPercent)
{
    if (HealthPercent < 0.2f)
        Phase3();
    else if (HealthPercent < 0.5f)
        Phase2();
    else
        Phase1();
}

void ANoxBossCharacter::Phase1()
{
    // Базовая логика
}

void ANoxBossCharacter::Phase2()
{
    // Усиление
}

void ANoxBossCharacter::Phase3()
{
    // Берсерк
}
