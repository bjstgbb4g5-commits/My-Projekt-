
#pragma once

#include "CoreMinimal.h"
#include "SocialTypes.generated.h"

USTRUCT(BlueprintType)
struct FSocialRelationship
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<AActor> TargetNPC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Social")
	float Friendship = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Social")
	float Trust = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Social")
	float Fear = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Social")
	float Attraction = 0.0f;

	UPROPERTY()
	float LastInteractionTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FRumorRecord
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Rumor")
	TWeakObjectPtr<AActor> SourceNPC;

	UPROPERTY(BlueprintReadWrite, Category = "Rumor")
	TWeakObjectPtr<AActor> SubjectNPC;

	UPROPERTY(BlueprintReadWrite, Category = "Rumor")
	FString Description;

	UPROPERTY(BlueprintReadWrite, Category = "Rumor")
	FVector Location;

	UPROPERTY(BlueprintReadWrite, Category = "Rumor")
	float Importance = 0.5f;

	UPROPERTY(BlueprintReadWrite, Category = "Rumor")
	float Timestamp = 0.0f;
};
