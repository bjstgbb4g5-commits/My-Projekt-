#include "NPCActivityManager.h"
#include "Engine/World.h"

void UNPCActivityManager::Initialize(UWorld* InWorld)
{
    World = InWorld;
}

void UNPCActivityManager::Shutdown()
{
    ActivityPoints.Empty();
}

void UNPCActivityManager::RegisterActivityPoint(UNPCActivityPointComponent* Point)
{
    if (Point)
    {
        ActivityPoints.AddUnique(Point);
    }
}

void UNPCActivityManager::UnregisterActivityPoint(UNPCActivityPointComponent* Point)
{
    ActivityPoints.Remove(Point);
}

UNPCActivityPointComponent* UNPCActivityManager::FindActivityPoint(ENPCActivityType Activity, const FVector& NPCLocation, float MaxDistance)
{
    UNPCActivityPointComponent* BestPoint = nullptr;
    float BestDistance = FLT_MAX;

    for (UNPCActivityPointComponent* Point : ActivityPoints)
    {
        if (!Point) continue;
        if (!Point->HasFreeSlot(Activity)) continue;

        FVector PointLoc = Point->GetComponentLocation();
        float Dist = FVector::Dist(NPCLocation, PointLoc);
        if (Dist > MaxDistance) continue;

        if (Dist < BestDistance)
        {
            BestDistance = Dist;
            BestPoint = Point;
        }
    }
    return BestPoint;
}

bool UNPCActivityManager::RequestActivity(AActor* NPC, ENPCActivityType Activity, const FVector& NPCLocation, UNPCActivityPointComponent*& OutPoint)
{
    UNPCActivityPointComponent* Point = FindActivityPoint(Activity, NPCLocation, 2000.0f);
    if (!Point) return false;

    if (Point->TryReserveSlot(NPC, Activity))
    {
        OutPoint = Point;
        return true;
    }
    return false;
}

void UNPCActivityManager::Update(float DeltaTime)
{
    UpdateTimer += DeltaTime;
    if (UpdateTimer < UPDATE_INTERVAL) return;
    UpdateTimer = 0.0f;

    for (UNPCActivityPointComponent* Point : ActivityPoints)
    {
        if (Point)
        {
            for (FActivitySlot& Slot : Point->Slots)
            {
                if (!Slot.OccupyingNPC.IsValid())
                {
                    Slot.OccupyingNPC = nullptr;
                }
            }
        }
    }
}
