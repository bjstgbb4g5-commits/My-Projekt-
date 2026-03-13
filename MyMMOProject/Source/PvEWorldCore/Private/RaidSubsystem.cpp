#include "RaidSubsystem.h"

int32 URaidSubsystem::CreateRaid(const TArray<APlayerState*>& Players, ERaidDifficulty Difficulty)
{
    FRaidInstance Raid;
    Raid.RaidID = NextRaidID++;
    Raid.Players = Players;
    Raid.Difficulty = Difficulty;
    Raid.State = ERaidState::Waiting;
    Raid.BossIndex = 0;
    Raid.BossCount = 3;
    ActiveRaids.Add(Raid.RaidID, Raid);
    return Raid.RaidID;
}

void URaidSubsystem::StartRaid(int32 RaidID)
{
    FRaidInstance* Raid = ActiveRaids.Find(RaidID);
    if (!Raid) return;
    Raid->State = ERaidState::Active;
}

void URaidSubsystem::BossKilled(int32 RaidID)
{
    FRaidInstance* Raid = ActiveRaids.Find(RaidID);
    if (!Raid) return;
    Raid->BossIndex++;
    if (Raid->BossIndex >= Raid->BossCount)
    {
        Raid->State = ERaidState::Completed;
    }
}
