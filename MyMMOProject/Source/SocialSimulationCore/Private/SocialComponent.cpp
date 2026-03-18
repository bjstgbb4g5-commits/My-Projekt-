
#include "SocialComponent.h"
#include "SocialManager.h"
#include "SocialSubsystem.h"
#include "NPCMemoryComponent.h"
#include "GameFramework/Actor.h"

USocialComponent::USocialComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USocialComponent::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* World = GetWorld())
	{
		if (USocialSubsystem* Subsystem = World->GetSubsystem<USocialSubsystem>())
		{
			if (USocialManager* Manager = Subsystem->GetSocialManager())
			{
				Manager->RegisterComponent(this);
			}
		}
	}
}

void USocialComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (USocialSubsystem* Subsystem = World->GetSubsystem<USocialSubsystem>())
		{
			if (USocialManager* Manager = Subsystem->GetSocialManager())
			{
				Manager->UnregisterComponent(this);
			}
		}
	}
	Super::EndPlay(EndPlayReason);
}

FSocialRelationship* USocialComponent::GetOrCreateRelationship(AActor* OtherNPC)
{
	if (!OtherNPC) return nullptr;

	TWeakObjectPtr<AActor> Key(OtherNPC);
	if (FSocialRelationship* Existing = Relationships.Find(Key))
	{
		return Existing;
	}

	if (Relationships.Num() >= MaxRelationships)
	{
		EnforceMaxRelationships();
		if (Relationships.Num() >= MaxRelationships)
			return nullptr;
	}

	FSocialRelationship NewRel;
	NewRel.TargetNPC = Key;
	NewRel.LastInteractionTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	Relationships.Add(Key, NewRel);
	return &Relationships[Key];
}

void USocialComponent::ModifyFriendship(AActor* OtherNPC, float Delta)
{
	if (FSocialRelationship* Rel = GetOrCreateRelationship(OtherNPC))
	{
		Rel->Friendship = FMath::Clamp(Rel->Friendship + Delta, -100.0f, 100.0f);
		Rel->LastInteractionTime = GetWorld()->GetTimeSeconds();
	}
}

void USocialComponent::ModifyTrust(AActor* OtherNPC, float Delta)
{
	if (FSocialRelationship* Rel = GetOrCreateRelationship(OtherNPC))
	{
		Rel->Trust = FMath::Clamp(Rel->Trust + Delta, 0.0f, 1.0f);
		Rel->LastInteractionTime = GetWorld()->GetTimeSeconds();
	}
}

void USocialComponent::ModifyFear(AActor* OtherNPC, float Delta)
{
	if (FSocialRelationship* Rel = GetOrCreateRelationship(OtherNPC))
	{
		Rel->Fear = FMath::Clamp(Rel->Fear + Delta, 0.0f, 1.0f);
		Rel->LastInteractionTime = GetWorld()->GetTimeSeconds();
	}
}

void USocialComponent::ModifyAttraction(AActor* OtherNPC, float Delta)
{
	if (FSocialRelationship* Rel = GetOrCreateRelationship(OtherNPC))
	{
		Rel->Attraction = FMath::Clamp(Rel->Attraction + Delta, 0.0f, 1.0f);
		Rel->LastInteractionTime = GetWorld()->GetTimeSeconds();
	}
}

float USocialComponent::GetFriendship(AActor* OtherNPC) const
{
	if (!OtherNPC) return 0.0f;
	if (const FSocialRelationship* Rel = Relationships.Find(OtherNPC))
	{
		return Rel->Friendship;
	}
	return 0.0f;
}

void USocialComponent::UpdateLastInteraction(AActor* OtherNPC)
{
	if (FSocialRelationship* Rel = Relationships.Find(OtherNPC))
	{
		Rel->LastInteractionTime = GetWorld()->GetTimeSeconds();
	}
}

void USocialComponent::ReceiveRumor(const FRumorRecord& Rumor)
{
	if (UNPCMemoryComponent* MemoryComp = GetOwner()->FindComponentByClass<UNPCMemoryComponent>())
	{
		FNPCMemoryRecord Memory;
		Memory.Type = ENPCMemoryType::Rumor;
		Memory.RelatedActor = Rumor.SubjectNPC.Get();
		Memory.Location = Rumor.Location;
		Memory.Timestamp = Rumor.Timestamp;
		Memory.Importance = Rumor.Importance;
		Memory.Description = Rumor.Description;
		MemoryComp->AddMemory(Memory);
	}
}

void USocialComponent::UpdateRelationships(float CurrentTime, float DeltaTime)
{
	for (auto& Pair : Relationships)
	{
		if (!Pair.Key.IsValid()) continue;
		ApplyDecay(Pair.Value, DeltaTime);
	}

	TArray<TWeakObjectPtr<AActor>> ToRemove;
	for (const auto& Pair : Relationships)
	{
		if (!Pair.Key.IsValid())
		{
			ToRemove.Add(Pair.Key);
		}
	}
	for (const auto& Key : ToRemove)
	{
		Relationships.Remove(Key);
	}

	if (RelationshipDecayTime > 0.0f)
	{
		ToRemove.Empty();
		for (const auto& Pair : Relationships)
		{
			if (CurrentTime - Pair.Value.LastInteractionTime > RelationshipDecayTime)
			{
				ToRemove.Add(Pair.Key);
			}
		}
		for (const auto& Key : ToRemove)
		{
			Relationships.Remove(Key);
		}
	}

	EnforceMaxRelationships();
}

void USocialComponent::ApplyDecay(FSocialRelationship& Rel, float DeltaTime)
{
	if (Rel.Friendship > 0.0f)
		Rel.Friendship = FMath::Max(0.0f, Rel.Friendship - DecayRate * DeltaTime);
	else if (Rel.Friendship < 0.0f)
		Rel.Friendship = FMath::Min(0.0f, Rel.Friendship + DecayRate * DeltaTime);

	Rel.Trust = FMath::Max(0.0f, Rel.Trust - DecayRate * DeltaTime * 0.5f);
	Rel.Fear = FMath::Max(0.0f, Rel.Fear - DecayRate * DeltaTime * 0.5f);
	Rel.Attraction = FMath::Max(0.0f, Rel.Attraction - DecayRate * DeltaTime * 0.5f);
}

void USocialComponent::EnforceMaxRelationships()
{
	if (Relationships.Num() <= MaxRelationships) return;

	TArray<TPair<TWeakObjectPtr<AActor>, FSocialRelationship>> Pairs;
	for (const auto& Pair : Relationships)
	{
		Pairs.Add(Pair);
	}
	Pairs.Sort([](const auto& A, const auto& B)
	{
		return A.Value.LastInteractionTime < B.Value.LastInteractionTime;
	});

	int32 Excess = Pairs.Num() - MaxRelationships;
	for (int32 i = 0; i < Excess; ++i)
	{
		Relationships.Remove(Pairs[i].Key);
	}
}
