#pragma once

#include "CoreMinimal.h"
#include "PvETypes.generated.h"

UENUM(BlueprintType)
enum class EPvEZoneType : uint8
{
    Green,
    Blue,
    Red,
    Black
};

UENUM(BlueprintType)
enum class EPvEContentType : uint8
{
    Wildlife,
    BanditCamp,
    Dungeon,
    RaidBoss,
    WorldBoss,
    CultistZone,
    ApostleZone
};

UENUM(BlueprintType)
enum class EApostleType : uint8
{
    War,
    Plague,
    Void,
    Storm,
    Flame,
    Nox
};

UENUM(BlueprintType)
enum class EDungeonDifficulty : uint8
{
    Normal,
    Elite,
    Mythic
};

UENUM(BlueprintType)
enum class EDungeonState : uint8
{
    Waiting,
    Active,
    BossFight,
    Completed,
    Failed
};

UENUM(BlueprintType)
enum class ERaidDifficulty : uint8
{
    Normal,
    Heroic,
    Mythic
};

UENUM(BlueprintType)
enum class ERaidState : uint8
{
    Waiting,
    Active,
    BossFight,
    Completed,
    Failed
};

UENUM(BlueprintType)
enum class EWorldBossState : uint8
{
    Dormant,
    Spawned,
    Fighting,
    Dead
};

UENUM(BlueprintType)
enum class EApostleState : uint8
{
    Dormant,
    Active,
    Dead
};

UENUM(BlueprintType)
enum class EEcosystemFaction : uint8
{
    Wildlife,
    Bandits,
    Cultists,
    Monsters
};

UENUM(BlueprintType)
enum class EEcosystemBehavior : uint8
{
    Passive,
    Territorial,
    Predator,
    Patrol,
    Ritual
};

USTRUCT(BlueprintType)
struct FPvEZone
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ZoneID = -1;

    UPROPERTY()
    EPvEZoneType ZoneType = EPvEZoneType::Green;

    UPROPERTY()
    TArray<EPvEContentType> Content;
};

USTRUCT(BlueprintType)
struct FEcosystemZone
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ZoneID = -1;

    UPROPERTY()
    FVector Center;

    UPROPERTY()
    float Radius = 0.f;

    UPROPERTY()
    EEcosystemFaction OwnerFaction = EEcosystemFaction::Wildlife;

    UPROPERTY()
    int32 Population = 0;

    UPROPERTY()
    int32 MaxPopulation = 30;

    UPROPERTY()
    float RespawnTime = 60.f;
};

USTRUCT(BlueprintType)
struct FDungeonInstance
{
    GENERATED_BODY()

    UPROPERTY()
    int32 InstanceID = -1;

    UPROPERTY()
    EDungeonDifficulty Difficulty = EDungeonDifficulty::Normal;

    UPROPERTY()
    EDungeonState State = EDungeonState::Waiting;

    UPROPERTY()
    TArray<class APlayerState*> Players;

    UPROPERTY()
    int32 CurrentRoom = 0;

    UPROPERTY()
    bool bBossAlive = true;
};

USTRUCT(BlueprintType)
struct FRaidInstance
{
    GENERATED_BODY()

    UPROPERTY()
    int32 RaidID = -1;

    UPROPERTY()
    ERaidDifficulty Difficulty = ERaidDifficulty::Normal;

    UPROPERTY()
    ERaidState State = ERaidState::Waiting;

    UPROPERTY()
    TArray<class APlayerState*> Players;

    UPROPERTY()
    int32 BossIndex = 0;

    UPROPERTY()
    int32 BossCount = 3;
};

USTRUCT(BlueprintType)
struct FWorldBossData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 BossID = -1;

    UPROPERTY()
    FVector SpawnLocation;

    UPROPERTY()
    float MaxHealth = 0.f;

    UPROPERTY()
    float CurrentHealth = 0.f;

    UPROPERTY()
    EWorldBossState State = EWorldBossState::Dormant;
};

USTRUCT(BlueprintType)
struct FApostleData
{
    GENERATED_BODY()

    UPROPERTY()
    EApostleType Apostle = EApostleType::War;

    UPROPERTY()
    FVector SpawnLocation;

    UPROPERTY()
    float MaxHealth = 0.f;

    UPROPERTY()
    bool bAlive = true;
};

USTRUCT(BlueprintType)
struct FNoxFragment
{
    GENERATED_BODY()

    UPROPERTY()
    int32 FragmentID = -1;

    UPROPERTY()
    FVector Location;

    UPROPERTY()
    bool bDiscovered = false;
};
