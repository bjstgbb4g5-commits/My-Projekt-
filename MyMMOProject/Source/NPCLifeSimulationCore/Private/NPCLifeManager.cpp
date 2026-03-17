#include "NPCLifeManager.h"
#include "NPCLifeComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

void UNPCLifeManager::Initialize()
{
}

void UNPCLifeManager::Shutdown()
{
    NPCComponents.Empty();
    NPCDatas.Empty();
}

void UNPCLifeManager::RegisterNPC(AActor* NPC, const FNPCLifeData& Data)
{
    if (!NPC) return;

    UNPCLifeComponent* Comp = NPC->FindComponentByClass<UNPCLifeComponent>();
    if (!Comp)
    {
        Comp = NewObject<UNPCLifeComponent>(NPC);
        Comp->RegisterComponent();
    }
    Comp->InitializeFromData(Data);

    NPCComponents.Add(Comp);
    NPCDatas.Add(NPC, Data);
}

void UNPCLifeManager::UnregisterNPC(AActor* NPC)
{
    if (!NPC) return;

    UNPCLifeComponent* Comp = NPC->FindComponentByClass<UNPCLifeComponent>();
    if (Comp)
    {
        NPCComponents.Remove(Comp);
        NPCDatas.Remove(NPC);
    }
}

int32 UNPCLifeManager::CalculateLOD(AActor* NPCActor) const
{
    UWorld* World = NPCActor->GetWorld();
    if (!World) return 3;

    float MinDist = FLT_MAX;
    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->GetPawn())
        {
            float Dist = FVector::Dist(NPCActor->GetActorLocation(), PC->GetPawn()->GetActorLocation());
            MinDist = FMath::Min(MinDist, Dist);
        }
    }

    if (MinDist < 2000.0f) return 0;
    if (MinDist < 5000.0f) return 1;
    if (MinDist < 15000.0f) return 2;
    return 3;
}

void UNPCLifeManager::UpdateSimulation(float DeltaTime, float CurrentHour)
{
    for (UNPCLifeComponent* Comp : NPCComponents)
    {
        if (!Comp) continue;

        AActor* Owner = Comp->GetOwner();
        if (!Owner) continue;

        int32 LOD = CalculateLOD(Owner) + LODBias;
        LOD = FMath::Clamp(LOD, 0, 3);
        Comp->SetLODLevel(LOD);

        if (LOD <= 1)
        {
            Comp->UpdateLife(CurrentHour);
        }
        if (LOD == 0)
        {
            Comp->PerformTask(DeltaTime);
        }
    }
}
