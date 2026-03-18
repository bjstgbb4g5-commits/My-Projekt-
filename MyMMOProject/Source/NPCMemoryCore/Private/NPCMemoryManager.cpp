
#include "NPCMemoryManager.h"

void UNPCMemoryManager::RegisterComponent(UNPCMemoryComponent* MemoryComp)
{
	if (MemoryComp)
	{
		RegisteredComponents.AddUnique(MemoryComp);
	}
}

void UNPCMemoryManager::UnregisterComponent(UNPCMemoryComponent* MemoryComp)
{
	RegisteredComponents.RemoveAll([MemoryComp](const TWeakObjectPtr<UNPCMemoryComponent>& Ptr)
	{
		return Ptr.Get() == MemoryComp || !Ptr.IsValid();
	});
}

void UNPCMemoryManager::BroadcastEvent(const FNPCMemoryRecord& Event, float Radius, const FVector& Origin)
{
	for (const TWeakObjectPtr<UNPCMemoryComponent>& CompPtr : RegisteredComponents)
	{
		if (!CompPtr.IsValid()) continue;

		UNPCMemoryComponent* Comp = CompPtr.Get();
		if (Radius <= 0.0f)
		{
			Comp->AddMemory(Event);
		}
		else
		{
			AActor* Owner = Comp->GetOwner();
			if (Owner && FVector::Dist(Owner->GetActorLocation(), Origin) <= Radius)
			{
				Comp->AddMemory(Event);
			}
		}
	}
}

void UNPCMemoryManager::UpdateAllMemories(float CurrentTime)
{
	for (const TWeakObjectPtr<UNPCMemoryComponent>& CompPtr : RegisteredComponents)
	{
		if (CompPtr.IsValid())
		{
			CompPtr->UpdateMemory(CurrentTime);
		}
	}

	RegisteredComponents.RemoveAll([](const TWeakObjectPtr<UNPCMemoryComponent>& Ptr)
	{
		return !Ptr.IsValid();
	});
}
