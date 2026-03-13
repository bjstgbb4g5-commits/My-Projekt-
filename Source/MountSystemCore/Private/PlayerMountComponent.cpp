#include "PlayerMountComponent.h"
#include "Net/UnrealNetwork.h"
#include "MountActor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"

UPlayerMountComponent::UPlayerMountComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
    SpawnedMountActor = nullptr;
}

void UPlayerMountComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UPlayerMountComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UPlayerMountComponent, OwnedMounts, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UPlayerMountComponent, ActiveMountID, COND_OwnerOnly);
}

bool UPlayerMountComponent::IsServer() const
{
    return GetOwner() && GetOwner()->HasAuthority();
}

void UPlayerMountComponent::AddMount(const FMountData& Mount)
{
    if (!IsServer()) return;
    OwnedMounts.Add(Mount);
}

void UPlayerMountComponent::RemoveMount(int32 MountID)
{
    if (!IsServer()) return;
    int32 Index = OwnedMounts.IndexOfByPredicate([MountID](const FMountData& M) { return M.MountID == MountID; });
    if (Index != INDEX_NONE)
    {
        OwnedMounts.RemoveAt(Index);
        if (ActiveMountID == MountID)
        {
            DespawnMount();
        }
    }
}

FMountData UPlayerMountComponent::GetMountData(int32 MountID) const
{
    for (const FMountData& M : OwnedMounts)
    {
        if (M.MountID == MountID)
            return M;
    }
    return FMountData();
}

bool UPlayerMountComponent::OwnsMount(int32 MountID) const
{
    return OwnedMounts.ContainsByPredicate([MountID](const FMountData& M) { return M.MountID == MountID; });
}

void UPlayerMountComponent::SummonMount(int32 MountID)
{
    if (!IsServer()) return;
    if (!OwnsMount(MountID)) return;

    if (SpawnedMountActor)
    {
        DespawnMount();
    }

    FMountData MountData = GetMountData(MountID);
    if (!MountData.bIsAlive) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    SpawnParams.Instigator = Cast<APawn>(GetOwner());

    FVector SpawnLocation = GetOwner()->GetActorLocation() + FVector(100.f, 0.f, 0.f);
    FRotator SpawnRotation = GetOwner()->GetActorRotation();

    AMountActor* MountActor = GetWorld()->SpawnActor<AMountActor>(AMountActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
    if (MountActor)
    {
        MountActor->SetMountData(MountData);
        MountActor->SetOwner(GetOwner());
        SpawnedMountActor = MountActor;
        ActiveMountID = MountID;

        AController* Controller = Cast<APawn>(GetOwner())->GetController();
        if (Controller)
        {
            Controller->Possess(MountActor);
        }
    }
}

void UPlayerMountComponent::DespawnMount()
{
    if (!IsServer()) return;
    if (SpawnedMountActor)
    {
        AController* Controller = SpawnedMountActor->GetController();
        if (Controller)
        {
            Controller->Possess(Cast<APawn>(GetOwner()));
        }
        SpawnedMountActor->Destroy();
        SpawnedMountActor = nullptr;
        ActiveMountID = -1;
    }
}
