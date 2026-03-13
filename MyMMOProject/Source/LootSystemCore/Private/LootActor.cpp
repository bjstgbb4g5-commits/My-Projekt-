#include "LootActor.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "PlayerCharacter.h"
#include "LootSubsystem.h"

ALootActor::ALootActor()
{
    bReplicates = true;
    bAlwaysRelevant = false;
    NetUpdateFrequency = 5.0f;

    InteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionTrigger"));
    RootComponent = InteractionTrigger;
    InteractionTrigger->SetBoxExtent(FVector(100.f, 100.f, 100.f));
    InteractionTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ALootActor::BeginPlay()
{
    Super::BeginPlay();
    if (HasAuthority())
    {
        GetWorld()->GetTimerManager().SetTimer(LifespanTimerHandle, this, &ALootActor::OnLifespanExpired, LifespanSeconds, false);
    }
}

void ALootActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (HasAuthority())
    {
        ULootSubsystem* LootSystem = GetWorld()->GetSubsystem<ULootSubsystem>();
        if (LootSystem)
        {
            LootSystem->CleanupLootArray();
        }
    }
}

void ALootActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ALootActor, Item);
}

void ALootActor::Pickup(APlayerController* Player)
{
    if (Player)
    {
        Server_Pickup(Player);
    }
}

void ALootActor::Server_Pickup_Implementation(APlayerController* Player)
{
    if (!HasAuthority()) return;
    if (!Player || !Player->GetPawn()) return;

    // Анти‑спам
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastPickupTime < 0.2f)
        return;
    LastPickupTime = CurrentTime;

    // Проверка расстояния
    if (FVector::Dist(Player->GetPawn()->GetActorLocation(), GetActorLocation()) > 200.0f)
        return;

    OnPickedUp(Player);
}

void ALootActor::OnPickedUp(APlayerController* Player)
{
    ULootSubsystem* LootSystem = GetWorld()->GetSubsystem<ULootSubsystem>();
    if (!LootSystem) return;

    APlayerCharacter* Character = Cast<APlayerCharacter>(Player->GetPawn());
    if (!Character) return;

    APlayerInventoryActor* Inventory = Character->InventoryActor;
    if (!Inventory) return;

    if (LootSystem->PickupLoot(this, Inventory))
    {
        // Уничтожение произойдёт внутри PickupLoot
    }
}

void ALootActor::OnLifespanExpired()
{
    if (HasAuthority())
    {
        Destroy();
    }
}
