#include "NPCCharacter.h"
#include "NPCLifeComponent.h"

ANPCCharacter::ANPCCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    LifeComponent = CreateDefaultSubobject<UNPCLifeComponent>(TEXT("NPCLifeComponent"));
}
