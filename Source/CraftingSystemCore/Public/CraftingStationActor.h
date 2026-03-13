#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CraftingTypes.h"
#include "CraftingStationActor.generated.h"

UCLASS()
class CRAFTINGSYSTEMCORE_API ACraftingStationActor : public AActor
{
    GENERATED_BODY()

public:
    ACraftingStationActor();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
    ECraftingStationType StationType = ECraftingStationType::None;
};
