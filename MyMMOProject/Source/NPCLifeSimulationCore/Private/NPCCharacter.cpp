#include "NPCCharacter.h"

#include "NPCLifeComponent.h"
#include "NPCActivityComponent.h"
#include "NPCPerceptionComponent.h"
#include "UtilityAIComponent.h"
#include "CrowdAgentComponent.h"
#include "NPCMemoryComponent.h"
#include "SocialComponent.h"

ANPCCharacter::ANPCCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    /*
    Create core NPC components
    */

    LifeComponent =
        CreateDefaultSubobject<UNPCLifeComponent>(
            TEXT("NPCLifeComponent"));

    ActivityComponent =
        CreateDefaultSubobject<UNPCActivityComponent>(
            TEXT("NPCActivityComponent"));

    PerceptionComponent =
        CreateDefaultSubobject<UNPCPerceptionComponent>(
            TEXT("NPCPerceptionComponent"));

    UtilityAIComponent =
        CreateDefaultSubobject<UUtilityAIComponent>(
            TEXT("UtilityAIComponent"));

    CrowdAgentComponent =
        CreateDefaultSubobject<UCrowdAgentComponent>(
            TEXT("CrowdAgentComponent"));

    /*
    Advanced systems
    */

    MemoryComponent =
        CreateDefaultSubobject<UNPCMemoryComponent>(
            TEXT("MemoryComponent"));

    SocialComponent =
        CreateDefaultSubobject<USocialComponent>(
            TEXT("SocialComponent"));
}

void ANPCCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ANPCCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}
