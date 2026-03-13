#include "WorldBossCharacter.h"
#include "WorldBossSubsystem.h"
#include "Engine/World.h"

AWorldBossCharacter::AWorldBossCharacter()
{
}

void AWorldBossCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AWorldBossCharacter::InitializeHealth(float Health)
{
    if (HasAuthority())
    {
        CurrentHealth = Health;
        MaxHealth = Health;
    }
}

float AWorldBossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    if (HasAuthority() && CurrentHealth <= 0.f)
    {
        UWorldBossSubsystem* BossSystem = GetWorld()->GetSubsystem<UWorldBossSubsystem>();
        if (BossSystem)
        {
            BossSystem->KillWorldBoss(BossID);
        }
        // Здесь можно дать награды, запустить события
    }
    return ActualDamage;
}
