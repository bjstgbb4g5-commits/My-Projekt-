
#include "NPCMemorySubsystem.h"

void UNPCMemorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	MemoryManager = NewObject<UNPCMemoryManager>(this);
}

void UNPCMemorySubsystem::Deinitialize()
{
	MemoryManager = nullptr;
	Super::Deinitialize();
}

void UNPCMemorySubsystem::Tick(float DeltaTime)
{
	TimeSinceLastUpdate += DeltaTime;
	if (TimeSinceLastUpdate >= UpdateInterval)
	{
		TimeSinceLastUpdate = 0.0f;
		if (MemoryManager)
		{
			float CurrentTime = GetWorld()->GetTimeSeconds();
			MemoryManager->UpdateAllMemories(CurrentTime);
		}
	}
}

TStatId UNPCMemorySubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UNPCMemorySubsystem, STATGROUP_Tickables);
}
