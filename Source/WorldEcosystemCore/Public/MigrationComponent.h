#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MigrationComponent.generated.h"

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class WORLDECOSYSTEMCORE_API UMigrationComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFUNCTION()
    void StartMigration(FVector Target);

protected:
    FVector MigrationTarget;
};
