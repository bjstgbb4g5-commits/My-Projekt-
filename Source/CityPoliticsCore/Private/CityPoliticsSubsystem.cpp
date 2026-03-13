#include "CityPoliticsSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

void UCityPoliticsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UCityPoliticsSubsystem::Deinitialize()
{
    CityRegistry.Empty();
    CityRelations.Empty();
    Super::Deinitialize();
}

void UCityPoliticsSubsystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UCityPoliticsSubsystem, CityRelations);
}

void UCityPoliticsSubsystem::RegisterCityActor(int32 CityId, AActor* CityActor)
{
    if (CityActor)
        CityRegistry.Add(CityId, CityActor);
}

AActor* UCityPoliticsSubsystem::GetCityActor(int32 CityId) const
{
    if (const TWeakObjectPtr<AActor>* Found = CityRegistry.Find(CityId))
        return Found->Get();
    return nullptr;
}

void UCityPoliticsSubsystem::ModifyRelation(int32 CityA, int32 CityB, float Delta)
{
    if (!GetWorld()->IsServer()) return;

    for (FCityRelation& Rel : CityRelations)
    {
        if ((Rel.CityA == CityA && Rel.CityB == CityB) || (Rel.CityA == CityB && Rel.CityB == CityA))
        {
            Rel.RelationValue = FMath::Clamp(Rel.RelationValue + Delta, -100.f, 100.f);
            return;
        }
    }

    FCityRelation NewRel;
    NewRel.CityA = CityA;
    NewRel.CityB = CityB;
    NewRel.RelationValue = FMath::Clamp(Delta, -100.f, 100.f);
    NewRel.bWarActive = false;
    CityRelations.Add(NewRel);
}

bool UCityPoliticsSubsystem::CanDeclareWar(int32 CityA, int32 CityB) const
{
    for (const FCityRelation& Rel : CityRelations)
    {
        if ((Rel.CityA == CityA && Rel.CityB == CityB) || (Rel.CityA == CityB && Rel.CityB == CityA))
        {
            return Rel.RelationValue <= -70.f && !Rel.bWarActive;
        }
    }
    return false;
}

void UCityPoliticsSubsystem::DeclareWar(int32 CityA, int32 CityB)
{
    if (!GetWorld()->IsServer()) return;
    if (!CanDeclareWar(CityA, CityB)) return;

    for (FCityRelation& Rel : CityRelations)
    {
        if ((Rel.CityA == CityA && Rel.CityB == CityB) || (Rel.CityA == CityB && Rel.CityB == CityA))
        {
            Rel.bWarActive = true;
            break;
        }
    }
}
