#include "RaidBoss.h"
#include "RaidSubsystem.h"
#include "Engine/World.h"

ARaidBoss::ARaidBoss()
{
}

void ARaidBoss::BeginPlay()
{
    Super::BeginPlay();
}

void ARaidBoss::OnBossKilled()
{
    UWorld* World = GetWorld();
    if (World && World->IsServer())
    {
        URaidSubsystem* RaidSystem = World->GetSubsystem<URaidSubsystem>();
        if (RaidSystem && RaidID != -1)
        {
            RaidSystem->BossKilled(RaidID);
        }
    }
}
