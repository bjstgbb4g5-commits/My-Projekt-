#include "BossCharacter.h"

ABossCharacter::ABossCharacter()
{
    PhaseComponent = CreateDefaultSubobject<UBossPhaseComponent>(TEXT("PhaseComponent"));
}

void ABossCharacter::UpdateBossPhase(float HealthPercent)
{
    if (PhaseComponent)
    {
        PhaseComponent->UpdatePhase(HealthPercent);
    }
}
