#include "ApostleSubsystem.h"

void UApostleSubsystem::SpawnApostle(EApostleType Apostle, FVector Location)
{
    FApostleData Data;
    Data.Apostle = Apostle;
    Data.SpawnLocation = Location;
    Data.MaxHealth = 800000.f;
    Data.bAlive = true;
    ActiveApostles.Add(Apostle, Data);
}

void UApostleSubsystem::KillApostle(EApostleType Apostle)
{
    FApostleData* Data = ActiveApostles.Find(Apostle);
    if (!Data) return;
    Data->bAlive = false;
}
