#include "NPCCharacter.h"

#include "NPCLifeComponent.h"
#include "NPCActivityComponent.h"
#include "NPCPerceptionComponent.h"
#include "UtilityAIComponent.h"

#include "NPCPerceptionSubsystem.h"
#include "UtilityAISubsystem.h"

#include "Engine/World.h"

ANPCCharacter::ANPCCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    /*
    Create core NPC components
    */

    LifeComponent =
        CreateDefaultSubobject<UNPCLifeComponent>(TEXT("NPCLifeComponent"));

    ActivityComponent =
        CreateDefaultSubobject<UNPCActivityComponent>(TEXT("NPCActivityComponent"));

    PerceptionComponent =
        CreateDefaultSubobject<UNPCPerceptionComponent>(TEXT("NPCPerceptionComponent"));

    UtilityAIComponent =
        CreateDefaultSubobject<UUtilityAIComponent>(TEXT("UtilityAIComponent"));
}

void ANPCCharacter::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (!World) return;

    /*
    Register NPC in Perception system
    */

    if (UNPCPerceptionSubsystem* PerceptionSubsystem =
        World->GetSubsystem<UNPCPerceptionSubsystem>())
    {
        if (UNPCPerceptionManager* Manager =
            PerceptionSubsystem->GetManager())
        {
            Manager->RegisterNPC(this);
        }
    }

    /*
    Register NPC in Utility AI system
    */

    if (UUtilityAISubsystem* UtilitySubsystem =
        World->GetSubsystem<UUtilityAISubsystem>())
    {
        if (UUtilityAIManager* Manager =
            UtilitySubsystem->GetManager())
        {
            Manager->RegisterNPC(this);
        }
    }
}

void ANPCCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UWorld* World = GetWorld();

    if (World)
    {
        if (UNPCPerceptionSubsystem* PerceptionSubsystem =
            World->GetSubsystem<UNPCPerceptionSubsystem>())
        {
            if (UNPCPerceptionManager* Manager =
                PerceptionSubsystem->GetManager())
            {
                Manager->UnregisterNPC(this);
            }
        }

        if (UUtilityAISubsystem* UtilitySubsystem =
            World->GetSubsystem<UUtilityAISubsystem>())
        {
            if (UUtilityAIManager* Manager =
                UtilitySubsystem->GetManager())
            {
                Manager->UnregisterNPC(this);
            }
        }
    }

    Super::EndPlay(EndPlayReason);
}
