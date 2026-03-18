
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SocialTypes.h"
#include "SocialComponent.h"
#include "SocialManager.generated.h"

UCLASS()
class SOCIALSIMULATIONCORE_API USocialManager : public UObject
{
	GENERATED_BODY()

public:
	void RegisterComponent(USocialComponent* SocialComp);
	void UnregisterComponent(USocialComponent* SocialComp);

	void UpdateAllComponents(float DeltaTime);

	TArray<USocialComponent*> FindNearbyComponents(const FVector& Origin, float Radius) const;

	UFUNCTION(BlueprintCallable, Category = "Rumor")
	void BroadcastRumor(const FRumorRecord& Rumor, float Radius = 0.0f, const FVector& Origin = FVector::ZeroVector);

	const TArray<TWeakObjectPtr<USocialComponent>>& GetAllComponents() const { return RegisteredComponents; }

private:
	TArray<TWeakObjectPtr<USocialComponent>> RegisteredComponents;

	void CleanupInvalidComponents();
};
