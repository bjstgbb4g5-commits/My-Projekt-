#pragma once

#include "CoreMinimal.h"
#include "NPCLifeTypes.generated.h"

UENUM(BlueprintType)
enum class ENPCProfession : uint8
{
    None,
    Shopkeeper,
    Blacksmith,
    Farmer,
    Guard,
    Worker,
    Noble
};

UENUM(BlueprintType)
enum class ENPCDailyTask : uint8
{
    Idle,
    Work,
    Eat,
    Sleep,
    Socialize,
    Travel
};

USTRUCT(BlueprintType)
struct FNPCScheduleEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    float StartHour = 0.0f;

    UPROPERTY(EditAnywhere)
    ENPCDailyTask Task = ENPCDailyTask::Idle;

    UPROPERTY(EditAnywhere)
    AActor* TargetLocation = nullptr;
};

USTRUCT(BlueprintType)
struct FNPCLifeData
{
    GENERATED_BODY()

    UPROPERTY()
    FString NPCName;

    UPROPERTY()
    int32 Age = 30;

    UPROPERTY()
    ENPCProfession Profession = ENPCProfession::None;

    UPROPERTY()
    AActor* HomeLocation = nullptr;

    UPROPERTY()
    AActor* Workplace = nullptr;

    UPROPERTY()
    TArray<FNPCScheduleEntry> Schedule;

    UPROPERTY()
    TArray<AActor*> Friends;
};
