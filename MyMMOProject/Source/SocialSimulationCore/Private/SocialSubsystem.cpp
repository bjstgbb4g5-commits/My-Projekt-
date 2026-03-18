
#include "SocialSubsystem.h"

void USocialSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SocialManager = NewObject<USocialManager>(this);
}

void USocialSubsystem::Deinitialize()
{
	SocialManager = nullptr;
	Super::Deinitialize();
}

void USocialSubsystem::Tick(float DeltaTime)
{
	TimeSinceLastUpdate += DeltaTime;
	if (TimeSinceLastUpdate >= UpdateInterval)
	{
		TimeSinceLastUpdate = 0.0f;
		if (SocialManager)
		{
			SocialManager->UpdateAllComponents(DeltaTime);
		}
	}
}

TStatId USocialSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(USocialSubsystem, STATGROUP_Tickables);
}
