#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonTypes.h"
#include "DungeonGenerator.generated.h"

USTRUCT(BlueprintType)
struct FDungeonRoom
{
    GENERATED_BODY()

    UPROPERTY()
    EDungeonRoomType RoomType = EDungeonRoomType::Entrance;

    UPROPERTY()
    int32 EnemyCount = 0;
};

UCLASS()
class DUNGEONSYSTEMCORE_API UDungeonGenerator : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    TArray<FDungeonRoom> GenerateDungeon(int32 Seed, int32 NumRooms = 5);
};
