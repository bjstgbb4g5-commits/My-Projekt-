#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HouseTypes.h"
#include "HouseSaveGame.generated.h"

UCLASS()
class HOUSESYSTEMCORE_API UHouseSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<FHouseData> Houses;

    UPROPERTY()
    int32 NextHouseId = 1;
};
