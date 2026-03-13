#include "ResourceNodeSubsystem.h"
#include "ResourceWorldState.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "TerritoryManagerSubsystem.h"
#include "AIDirectorCore/Public/AIWorldDirectorSubsystem.h"
#include "MarketplaceSystemCore/Public/MarketplaceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Misc/ScopeLock.h"

UResourceNodeSubsystem::UResourceNodeSubsystem()
{
}

void UResourceNodeSubsystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // Репликация через WorldStateActor
}

void UResourceNodeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (!IsServer()) return;

    TerritorySystem = GetWorld()->GetSubsystem<UTerritoryManagerSubsystem>();
    AIDirector = GetWorld()->GetSubsystem<UAIWorldDirectorSubsystem>();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = TEXT("ResourceWorldState");
    WorldStateActor = GetWorld()->SpawnActor<AResourceWorldState>(AResourceWorldState::StaticClass(), SpawnParams);
    if (!WorldStateActor)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn ResourceWorldState!"));
    }

    LoadResourceState();

    if (ResourceNodes.Num() == 0)
    {
        // Генерация начальных узлов
        for (int32 i = 0; i < 100 && ResourceNodes.Num() < MAX_RESOURCE_NODES; ++i)
        {
            FResourceNode Node;
            Node.NodeID = NextNodeID++;
            Node.ResourceType = static_cast<EWorldResourceType>(FMath::RandRange(0,5));
            Node.Location = FVector(FMath::RandRange(-50000,50000), FMath::RandRange(-50000,50000), 0);
            Node.MaxAmount = 1000;
            Node.CurrentAmount = Node.MaxAmount;
            Node.RegenerationRate = 0.01f;
            Node.State = EResourceNodeState::Available;
            Node.TerritoryID = FMath::RandRange(1,10);
            ResourceNodes.Add(Node.NodeID, Node);
        }
        UpdateTerritoryIndex();
    }

    if (WorldStateActor)
    {
        WorldStateActor->ResourceNodes = ResourceNodes;
    }
}

void UResourceNodeSubsystem::Deinitialize()
{
    SaveResourceState();

    FScopeLock Lock(&ResourceMutex);
    ResourceNodes.Empty();
    TerritoryNodes.Empty();
    RegenAccumulators.Empty();

    if (WorldStateActor && IsValid(WorldStateActor))
    {
        WorldStateActor->Destroy();
        WorldStateActor = nullptr;
    }
    Super::Deinitialize();
}

bool UResourceNodeSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UResourceNodeSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UResourceNodeSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UResourceNodeSubsystem, STATGROUP_Tickables);
}

void UResourceNodeSubsystem::Tick(float DeltaTime)
{
    FScopeLock Lock(&ResourceMutex);
    RegenerateNodes(DeltaTime);
}

void UResourceNodeSubsystem::RegenerateNodes(float DeltaTime)
{
    for (auto& Pair : ResourceNodes)
    {
        FResourceNode& Node = Pair.Value;
        if (Node.CurrentAmount <= 0)
            Node.State = EResourceNodeState::Regenerating;

        if (Node.State == EResourceNodeState::Regenerating)
        {
            float& Acc = RegenAccumulators.FindOrAdd(Node.NodeID);
            Acc += Node.MaxAmount * Node.RegenerationRate * DeltaTime;
            int32 RegenInt = FMath::FloorToInt(Acc);
            if (RegenInt > 0)
            {
                Node.CurrentAmount = FMath::Min(Node.MaxAmount, Node.CurrentAmount + RegenInt);
                Acc -= RegenInt;
            }
            if (Node.CurrentAmount >= Node.MaxAmount)
            {
                Node.CurrentAmount = Node.MaxAmount;
                Node.State = EResourceNodeState::Available;
                RegenAccumulators.Remove(Node.NodeID);
            }
        }
    }
}

int32 UResourceNodeSubsystem::HarvestResource(int32 NodeID, int32 Amount)
{
    if (!IsServer()) return 0;

    FScopeLock Lock(&ResourceMutex);
    FResourceNode* Node = ResourceNodes.Find(NodeID);
    if (!Node || Node->State != EResourceNodeState::Available) return 0;

    int32 Harvested = FMath::Min(Node->CurrentAmount, Amount);
    Node->CurrentAmount -= Harvested;

    if (Node->CurrentAmount <= 0)
        Node->State = EResourceNodeState::Depleted;

    if (WorldStateActor)
        WorldStateActor->ResourceNodes = ResourceNodes;

    NotifyMarketplace(NodeID, Harvested);
    return Harvested;
}

const FResourceNode* UResourceNodeSubsystem::GetNode(int32 NodeID) const
{
    FScopeLock Lock(&ResourceMutex);
    return ResourceNodes.Find(NodeID);
}

TArray<FResourceNode> UResourceNodeSubsystem::GetNodesInTerritory(int32 TerritoryID) const
{
    FScopeLock Lock(&ResourceMutex);
    TArray<FResourceNode> Result;
    if (const TArray<int32>* NodeIDs = TerritoryNodes.Find(TerritoryID))
    {
        for (int32 ID : *NodeIDs)
        {
            if (const FResourceNode* Node = ResourceNodes.Find(ID))
                Result.Add(*Node);
        }
    }
    return Result;
}

void UResourceNodeSubsystem::SpawnResourceNode(EWorldResourceType Type, const FVector& Location, int32 TerritoryID, int32 MaxAmount)
{
    FScopeLock Lock(&ResourceMutex);
    if (ResourceNodes.Num() >= MAX_RESOURCE_NODES) return;

    FResourceNode Node;
    Node.NodeID = NextNodeID++;
    Node.ResourceType = Type;
    Node.Location = Location;
    Node.MaxAmount = MaxAmount;
    Node.CurrentAmount = MaxAmount;
    Node.RegenerationRate = 0.01f;
    Node.State = EResourceNodeState::Available;
    Node.TerritoryID = TerritoryID;

    ResourceNodes.Add(Node.NodeID, Node);
    TerritoryNodes.FindOrAdd(TerritoryID).Add(Node.NodeID);

    if (WorldStateActor)
        WorldStateActor->ResourceNodes = ResourceNodes;
}

void UResourceNodeSubsystem::NotifyMarketplace(int32 NodeID, int32 Harvested)
{
    // Заглушка: уведомление Marketplace
}

void UResourceNodeSubsystem::UpdateTerritoryIndex()
{
    TerritoryNodes.Empty();
    for (const auto& Pair : ResourceNodes)
    {
        TerritoryNodes.FindOrAdd(Pair.Value.TerritoryID).Add(Pair.Key);
    }
}

void UResourceNodeSubsystem::SaveResourceState()
{
    if (!IsServer()) return;

    class UResourceSaveGame* SaveGame = Cast<UResourceSaveGame>(UGameplayStatics::CreateSaveGameObject(UResourceSaveGame::StaticClass()));
    if (!SaveGame) return;

    FScopeLock Lock(&ResourceMutex);
    ResourceNodes.GenerateValueArray(SaveGame->SavedNodes);
    SaveGame->SavedAccumulators = RegenAccumulators;
    SaveGame->SavedNextNodeID = NextNodeID;
    UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("ResourceState"), 0);
}

void UResourceNodeSubsystem::LoadResourceState()
{
    if (!IsServer()) return;

    if (!UGameplayStatics::DoesSaveGameExist(TEXT("ResourceState"), 0)) return;

    class UResourceSaveGame* LoadGame = Cast<UResourceSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ResourceState"), 0));
    if (!LoadGame) return;

    FScopeLock Lock(&ResourceMutex);
    ResourceNodes.Empty();
    for (const FResourceNode& Node : LoadGame->SavedNodes)
    {
        ResourceNodes.Add(Node.NodeID, Node);
    }
    RegenAccumulators = LoadGame->SavedAccumulators;
    NextNodeID = LoadGame->SavedNextNodeID;
    UpdateTerritoryIndex();
}
