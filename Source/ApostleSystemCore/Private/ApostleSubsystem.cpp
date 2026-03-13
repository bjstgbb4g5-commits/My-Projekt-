#include "ApostleSubsystem.h"
#include "ApostleCharacter.h"
#include "Engine/World.h"

UApostleSubsystem::UApostleSubsystem()
{
}

void UApostleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    // Здесь можно загрузить сохранённое состояние, если нужно
}

void UApostleSubsystem::Deinitialize()
{
    ActiveApostles.Empty();
    Super::Deinitialize();
}

bool UApostleSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UApostleSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UApostleSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UApostleSubsystem, STATGROUP_Tickables);
}

void UApostleSubsystem::Tick(float DeltaTime)
{
    // Можно проверять состояние апостолов, например, если они мертвы, инициировать респавн?
    // В зависимости от дизайна, апостолы могут не респавниться.
}

void UApostleSubsystem::SpawnApostle(EApostleType Apostle, FVector Location)
{
    if (!IsServer()) return;

    FApostleData Data;
    Data.Apostle = Apostle;
    Data.SpawnLocation = Location;
    Data.MaxHealth = 800000.f; // база
    Data.CurrentHealth = Data.MaxHealth;
    Data.bAlive = true;
    Data.State = EApostleState::Active;
    ActiveApostles.Add(Apostle, Data);

    // Спавним актор апостола
    UWorld* World = GetWorld();
    if (World)
    {
        FActorSpawnParameters SpawnParams;
        AApostleCharacter* ApostleChar = World->SpawnActor<AApostleCharacter>(AApostleCharacter::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);
        if (ApostleChar)
        {
            ApostleChar->ApostleType = Apostle;
            ApostleChar->InitializeHealth(Data.MaxHealth);
        }
    }
}

void UApostleSubsystem::KillApostle(EApostleType Apostle)
{
    if (!IsServer()) return;

    FApostleData* Data = ActiveApostles.Find(Apostle);
    if (!Data) return;

    Data->bAlive = false;
    Data->State = EApostleState::Dead;
}

bool UApostleSubsystem::IsApostleAlive(EApostleType Apostle) const
{
    const FApostleData* Data = ActiveApostles.Find(Apostle);
    return Data && Data->bAlive;
}

EApostleState UApostleSubsystem::GetApostleState(EApostleType Apostle) const
{
    const FApostleData* Data = ActiveApostles.Find(Apostle);
    return Data ? Data->State : EApostleState::Dormant;
}
