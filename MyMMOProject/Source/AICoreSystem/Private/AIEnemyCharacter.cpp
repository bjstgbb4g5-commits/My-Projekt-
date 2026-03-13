#include "AIEnemyCharacter.h"

AAIEnemyCharacter::AAIEnemyCharacter()
{
    ThreatComponent = CreateDefaultSubobject<UThreatComponent>(TEXT("ThreatComponent"));
}

void AAIEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
}
