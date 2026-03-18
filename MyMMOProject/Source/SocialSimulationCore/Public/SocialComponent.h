
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SocialTypes.h"
#include "SocialComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOCIALSIMULATIONCORE_API USocialComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USocialComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Social")
	int32 MaxRelationships = 32;

	UPROPERTY(EditDefaultsOnly, Category = "Social")
	float RelationshipDecayTime = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Social")
	float DecayRate = 0.1f;

	UFUNCTION(BlueprintCallable, Category = "Social")
	FSocialRelationship* GetOrCreateRelationship(AActor* OtherNPC);

	UFUNCTION(BlueprintCallable, Category = "Social")
	void ModifyFriendship(AActor* OtherNPC, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Social")
	void ModifyTrust(AActor* OtherNPC, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Social")
	void ModifyFear(AActor* OtherNPC, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Social")
	void ModifyAttraction(AActor* OtherNPC, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Social")
	float GetFriendship(AActor* OtherNPC) const;

	UFUNCTION(BlueprintCallable, Category = "Social")
	void UpdateLastInteraction(AActor* OtherNPC);

	const TMap<TWeakObjectPtr<AActor>, FSocialRelationship>& GetRelationships() const { return Relationships; }

	UFUNCTION(BlueprintCallable, Category = "Rumor")
	void ReceiveRumor(const FRumorRecord& Rumor);

protected:
	UPROPERTY()
	TMap<TWeakObjectPtr<AActor>, FSocialRelationship> Relationships;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void UpdateRelationships(float CurrentTime, float DeltaTime);

private:
	void ApplyDecay(FSocialRelationship& Rel, float DeltaTime);
	void EnforceMaxRelationships();

	friend class USocialManager;
};
