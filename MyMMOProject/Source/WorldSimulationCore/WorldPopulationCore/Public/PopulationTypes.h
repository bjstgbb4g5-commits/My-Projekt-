#pragma once

#include "CoreMinimal.h"
#include "PopulationTypes.generated.h"

UENUM(BlueprintType)
enum class EPopulationType : uint8
{
    Wildlife,
    BanditCamp,
    Village,
    WorldBoss,
    NPCPatrol
};

USTRUCT(BlueprintType)
struct FWildlifeSpawnData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RegionID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 CreatureCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSubclassOf<class ACharacter>> CreatureClasses;
};

USTRUCT(BlueprintType)
struct FBanditCampData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 CampID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 EnemyCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bCampCleared = false;
};

USTRUCT(BlueprintType)
struct FVillageSpawnData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString VillageName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Population = 0;
};

USTRUCT(BlueprintType)
struct FWorldBossSpawnData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 BossID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class ABossCharacter> BossClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float RespawnTime = 86400.f; // 24 часа
};

USTRUCT(BlueprintType)
struct FNPCPatrolData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FVector> PatrolPoints;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 GuardCount = 2;
};
