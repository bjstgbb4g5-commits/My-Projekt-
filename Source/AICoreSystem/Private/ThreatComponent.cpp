#include "ThreatComponent.h"

UThreatComponent::UThreatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(false);
}

void UThreatComponent::AddThreat(AActor* Target, float Amount)
{
    if (!Target || Amount <= 0.f) return;

    for (FThreatEntry& Entry : ThreatList)
    {
        if (Entry.Target == Target)
        {
            Entry.ThreatValue += Amount;
            SortThreatList();
            return;
        }
    }

    FThreatEntry NewEntry;
    NewEntry.Target = Target;
    NewEntry.ThreatValue = Amount;
    ThreatList.Add(NewEntry);
    SortThreatList();
}

void UThreatComponent::RemoveThreat(AActor* Target)
{
    ThreatList.RemoveAll([Target](const FThreatEntry& Entry)
    {
        return Entry.Target == Target;
    });
}

AActor* UThreatComponent::GetTopThreat() const
{
    if (ThreatList.Num() == 0) return nullptr;
    return ThreatList[0].Target;
}

TArray<AActor*> UThreatComponent::GetTargetsInOrder() const
{
    TArray<AActor*> Result;
    for (const FThreatEntry& Entry : ThreatList)
    {
        Result.Add(Entry.Target);
    }
    return Result;
}

void UThreatComponent::ResetThreat()
{
    ThreatList.Empty();
}

void UThreatComponent::SortThreatList()
{
    ThreatList.Sort([](const FThreatEntry& A, const FThreatEntry& B)
    {
        return A.ThreatValue > B.ThreatValue;
    });
}
