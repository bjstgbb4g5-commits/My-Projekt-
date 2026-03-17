#include "NPCPatrolSpawner.h"

TArray<FNPCPatrolData> UNPCPatrolSpawner::GenerateNPCPatrols(UWorldGeneratorSubsystem* WorldGen)
{
    TArray<FNPCPatrolData> Result;
    if (!WorldGen) return Result;

    // Простейший пример: вдоль каждой дороги создаём патруль
    for (const auto& Road : WorldGen->Roads)
    {
        if (Road.Num() < 2) continue;
        FNPCPatrolData Patrol;
        Patrol.PatrolPoints = Road;
        Patrol.GuardCount = FMath::RandRange(2, 4);
        Result.Add(Patrol);
    }
    return Result;
}
