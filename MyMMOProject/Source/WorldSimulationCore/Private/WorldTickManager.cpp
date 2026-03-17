#include "WorldTickManager.h"

FWorldTickManager::FWorldTickManager()
    : SimulationTime(0.0f)
    , Accumulator(0.0f)
{
}

void FWorldTickManager::Tick(float DeltaTime)
{
    Accumulator += DeltaTime;
}

bool FWorldTickManager::ShouldRunStep()
{
    if (Accumulator >= FIXED_STEP)
    {
        Accumulator -= FIXED_STEP;
        SimulationTime += FIXED_STEP;
        return true;
    }
    return false;
}
