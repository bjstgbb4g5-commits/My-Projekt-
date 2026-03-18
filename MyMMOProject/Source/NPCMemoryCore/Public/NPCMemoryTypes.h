
#pragma once

#include "CoreMinimal.h"
#include "NPCMemoryTypes.generated.h"

UENUM(BlueprintType)
enum class ENPCMemoryType : uint8
{
	Event   UMETA(DisplayName = "Event"),
	Actor   UMETA(DisplayName = "Actor"),
	Location UMETA(DisplayName = "Location"),
	Rumor   UMETA(DisplayName = "Rumor")
};

USTRUCT(BlueprintType)
struct FNPCMemoryRecord
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Memory")
	ENPCMemoryType Type = ENPCMemoryType::Event;

	UPROPERTY(BlueprintReadWrite, Category = "Memory")
	TWeakObjectPtr<AActor> RelatedActor = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Memory")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Memory")
	float Timestamp = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Memory")
	float Importance = 0.5f;

	UPROPERTY(BlueprintReadWrite, Category = "Memory")
	FString Description = TEXT("");
};
