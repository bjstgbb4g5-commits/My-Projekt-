#include "ApostleCharacter.h"
#include "ApostleSubsystem.h"
#include "Engine/World.h"

AApostleCharacter::AApostleCharacter()
{
}

void AApostleCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AApostleCharacter::InitializeHealth(float Health)
{
    if (HasAuthority())
    {
        CurrentHealth = Health;
        MaxHealth = Health;
    }
}

float AApostleCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    if (HasAuthority() && CurrentHealth <= 0.f)
    {
        UApostleSubsystem* ApostleSystem = GetWorld()->GetSubsystem<UApostleSubsystem>();
        if (ApostleSystem)
        {
            ApostleSystem->KillApostle(ApostleType);
        }
        // Здесь можно выдать награды, артефакты
    }
    return ActualDamage;
}
