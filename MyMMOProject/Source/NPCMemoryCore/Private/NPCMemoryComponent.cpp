
#include "NPCMemoryComponent.h"
#include "NPCMemorySubsystem.h"

UNPCMemoryComponent::UNPCMemoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNPCMemoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* World = GetWorld())
	{
		if (UNPCMemorySubsystem* Subsystem = World->GetSubsystem<UNPCMemorySubsystem>())
		{
			if (UNPCMemoryManager* Manager = Subsystem->GetMemoryManager())
			{
				Manager->RegisterComponent(this);
			}
		}
	}
}

void UNPCMemoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (UNPCMemorySubsystem* Subsystem = World->GetSubsystem<UNPCMemorySubsystem>())
		{
			if (UNPCMemoryManager* Manager = Subsystem->GetMemoryManager())
			{
				Manager->UnregisterComponent(this);
			}
		}
	}
	Super::EndPlay(EndPlayReason);
}

bool UNPCMemoryComponent::IsDuplicate(const FNPCMemoryRecord& NewMemory) const
{
	if (Memories.Num() == 0) return false;
	const FNPCMemoryRecord& Last = Memories.Last();
	if (Last.Description == NewMemory.Description &&
		(NewMemory.Timestamp - Last.Timestamp) < 5.0f)
	{
		return true;
	}
	return false;
}

void UNPCMemoryComponent::AddMemory(const FNPCMemoryRecord& NewMemory)
{
	if (IsDuplicate(NewMemory))
	{
		return;
	}

	if (MaxMemoryRecords > 0 && Memories.Num() >= MaxMemoryRecords)
	{
		LimitMemory(MaxMemoryRecords - 1);
	}

	Memories.Add(NewMemory);
}

void UNPCMemoryComponent::ForgetOldMemories(float MaxAge)
{
	float CurrentTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	Memories.RemoveAll([CurrentTime, MaxAge](const FNPCMemoryRecord& Record)
	{
		return (CurrentTime - Record.Timestamp) > MaxAge;
	});
}

void UNPCMemoryComponent::LimitMemory(int32 MaxRecords)
{
	if (Memories.Num() <= MaxRecords) return;

	Memories.Sort([](const FNPCMemoryRecord& A, const FNPCMemoryRecord& B)
	{
		if (A.Timestamp != B.Timestamp)
			return A.Timestamp < B.Timestamp;
		return A.Importance < B.Importance;
	});

	int32 Excess = Memories.Num() - MaxRecords;
	Memories.RemoveAt(0, Excess);
}

void UNPCMemoryComponent::ClearMemory()
{
	Memories.Empty();
}

void UNPCMemoryComponent::UpdateMemory(float CurrentTime)
{
	if (MemoryLifetime > 0.0f)
	{
		Memories.RemoveAll([CurrentTime, this](const FNPCMemoryRecord& Record)
		{
			return (CurrentTime - Record.Timestamp) > MemoryLifetime;
		});
	}
	if (MaxMemoryRecords > 0 && Memories.Num() > MaxMemoryRecords)
	{
		LimitMemory(MaxMemoryRecords);
	}
}
