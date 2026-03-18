
#include "SocialManager.h"

void USocialManager::RegisterComponent(USocialComponent* SocialComp)
{
	if (SocialComp)
	{
		RegisteredComponents.AddUnique(SocialComp);
	}
}

void USocialManager::UnregisterComponent(USocialComponent* SocialComp)
{
	RegisteredComponents.RemoveAll([SocialComp](const TWeakObjectPtr<USocialComponent>& Ptr)
	{
		return Ptr.Get() == SocialComp || !Ptr.IsValid();
	});
}

void USocialManager::UpdateAllComponents(float DeltaTime)
{
	CleanupInvalidComponents();

	float CurrentTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;

	for (const TWeakObjectPtr<USocialComponent>& CompPtr : RegisteredComponents)
	{
		if (USocialComponent* Comp = CompPtr.Get())
		{
			Comp->UpdateRelationships(CurrentTime, DeltaTime);
		}
	}
}

TArray<USocialComponent*> USocialManager::FindNearbyComponents(const FVector& Origin, float Radius) const
{
	TArray<USocialComponent*> Result;
	for (const TWeakObjectPtr<USocialComponent>& CompPtr : RegisteredComponents)
	{
		USocialComponent* Comp = CompPtr.Get();
		if (!Comp) continue;

		AActor* Owner = Comp->GetOwner();
		if (!Owner) continue;

		if (Radius <= 0.0f || FVector::Dist(Owner->GetActorLocation(), Origin) <= Radius)
		{
			Result.Add(Comp);
		}
	}
	return Result;
}

void USocialManager::BroadcastRumor(const FRumorRecord& Rumor, float Radius, const FVector& Origin)
{
	TArray<USocialComponent*> Targets = FindNearbyComponents(Origin, Radius);
	for (USocialComponent* Comp : Targets)
	{
		Comp->ReceiveRumor(Rumor);
	}
}

void USocialManager::CleanupInvalidComponents()
{
	RegisteredComponents.RemoveAll([](const TWeakObjectPtr<USocialComponent>& Ptr)
	{
		return !Ptr.IsValid();
	});
}
