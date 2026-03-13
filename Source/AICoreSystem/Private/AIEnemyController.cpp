#include "AIEnemyController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

AAIEnemyController::AAIEnemyController()
{
    PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");
    SetPerceptionComponent(*PerceptionComp);

    UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");
    SightConfig->SightRadius = 2000.f;
    SightConfig->LoseSightRadius = 2500.f;
    SightConfig->PeripheralVisionAngleDegrees = 90.f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
    PerceptionComp->ConfigureSense(*SightConfig);

    UAISenseConfig_Hearing* HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>("HearingConfig");
    HearingConfig->HearingRange = 1500.f;
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
    PerceptionComp->ConfigureSense(*HearingConfig);

    PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
    PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AAIEnemyController::OnTargetPerceptionUpdated);
}

void AAIEnemyController::BeginPlay()
{
    Super::BeginPlay();
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (InPawn)
    {
        ThreatComponent = InPawn->FindComponentByClass<UThreatComponent>();
    }
}

void AAIEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (!ThreatComponent) return;
    if (Stimulus.WasSuccessfullySensed())
    {
        ThreatComponent->AddThreat(Actor, 10.f);
    }
}
