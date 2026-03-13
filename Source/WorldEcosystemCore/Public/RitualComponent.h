#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RitualComponent.generated.h"

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class WORLDECOSYSTEMCORE_API URitualComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFUNCTION()
    void StartRitual();

    UFUNCTION()
    void CompleteRitual();

protected:
    float RitualTime = 60.f;
};
