#include "SiegeMachine.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"

ASiegeMachine::ASiegeMachine()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    bAlwaysRelevant = true;
    NetUpdateFrequency = 10.f;

    Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
    RootComponent = Collision;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
}

void ASiegeMachine::BeginPlay()
{
    Super::BeginPlay();
    if (HasAuthority())
    {
        CurrentHealth = Data.MaxHealth;
        CurrentReload = 0.f;
    }
}

void ASiegeMachine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASiegeMachine, Data);
    DOREPLIFETIME(ASiegeMachine, CurrentHealth);
    DOREPLIFETIME(ASiegeMachine, CurrentReload);
}

void ASiegeMachine::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (HasAuthority() && CurrentReload > 0.f)
    {
        CurrentReload -= DeltaTime;
    }
}

void ASiegeMachine::Fire()
{
    if (!HasAuthority()) return;
    if (CurrentReload > 0.f) return;
    // Здесь логика выстрела
    UE_LOG(LogTemp, Log, TEXT("SiegeMachine fired"));
    CurrentReload = Data.ReloadTime;
}

void ASiegeMachine::Server_Fire_Implementation()
{
    if (CurrentReload > 0.f) return;
    Fire(); // вызываем Fire на сервере
}

void ASiegeMachine::TakeDamage(float Damage, AController* Instigator)
{
    if (!HasAuthority()) return;

    float CappedDamage = FMath::Min(Damage, Data.MaxHealth * 0.25f);
    CurrentHealth -= CappedDamage;

    if (CurrentHealth <= 0.f)
    {
        OnDeath();
    }
}

void ASiegeMachine::Multicast_OnDestroyed_Implementation()
{
    // Визуальные эффекты, звук
}

void ASiegeMachine::OnDeath()
{
    Multicast_OnDestroyed();
    Destroy();
}
