#pragma once

#include "CoreMinimal.h"
#include "Math/UnrealMathUtility.h" // для FMath

class WORLDSIMULATIONCORE_API FWorldTickManager
{
public:
    static constexpr float FIXED_STEP = 0.5f;

    FWorldTickManager();

    void Tick(float DeltaTime);
    float GetSimulationTime() const { return SimulationTime; }
    bool ShouldRunStep();

    // Безопасный сброс – оставляет максимум один шаг
    void ResetAccumulator()
    {
        Accumulator = FMath::Min(Accumulator, FIXED_STEP);
    }

private:
    float SimulationTime = 0.0f;
    float Accumulator = 0.0f;
};
