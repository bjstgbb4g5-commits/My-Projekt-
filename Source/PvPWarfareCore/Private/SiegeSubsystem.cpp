#include "SiegeSubsystem.h"
#include "Engine/World.h"

void USiegeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    // Здесь можно загрузить дефолтные лимиты
}

bool USiegeSubsystem::CanSpawnSiegeMachine(ESiegeMachineType Type, int32 BattleID) const
{
    const TMap<ESiegeMachineType, int32>* Counts = BattleMachineCounts.Find(BattleID);
    int32 CurrentCount = Counts ? Counts->FindRef(Type) : 0;

    const FSiegeLimitConfig* Limits = BattleLimits.Find(BattleID);
    const FSiegeLimitConfig DefaultLimits;

    const FSiegeLimitConfig& L = Limits ? *Limits : DefaultLimits;

    int32 Max = 0;
    switch (Type)
    {
    case ESiegeMachineType::BatteringRam: Max = L.MaxRams; break;
    case ESiegeMachineType::Catapult: Max = L.MaxCatapults; break;
    case ESiegeMachineType::Ballista: Max = L.MaxBallistas; break;
    case ESiegeMachineType::ArcaneCannon: Max = L.MaxArcaneCannons; break;
    }
    return CurrentCount < Max;
}

void USiegeSubsystem::RegisterSiegeMachine(ESiegeMachineType Type, int32 BattleID)
{
    TMap<ESiegeMachineType, int32>& Counts = BattleMachineCounts.FindOrAdd(BattleID);
    Counts.FindOrAdd(Type)++;
}

void USiegeSubsystem::UnregisterSiegeMachine(ESiegeMachineType Type, int32 BattleID)
{
    TMap<ESiegeMachineType, int32>* Counts = BattleMachineCounts.Find(BattleID);
    if (Counts)
    {
        int32* Cnt = Counts->Find(Type);
        if (Cnt && --(*Cnt) <= 0)
        {
            Counts->Remove(Type);
        }
    }
}
