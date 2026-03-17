#include "CityEventManager.h"
#include "Engine/World.h"

void UCityEventManager::Initialize(UWorld* InWorld)
{
    World = InWorld;
}

void UCityEventManager::Shutdown()
{
    ActiveEvents.Empty();
}

void UCityEventManager::Update(float DeltaTime, float CurrentTime)
{
    EventSpawnTimer += DeltaTime;
}
