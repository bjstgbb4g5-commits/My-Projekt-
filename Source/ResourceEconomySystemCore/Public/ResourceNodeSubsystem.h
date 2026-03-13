#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "ResourceTypes.h"
#include "ResourceNodeSubsystem.generated.h"

class UTerritoryManagerSubsystem;
class UAIWorldDirectorSubsystem;
class AResourceWorldState;

UCLASS()
class RESOURCEECONOMYSYSTEMCORE_API UResourceNodeSubsystem
    : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UResourceNodeSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

    UFUNCTION(BlueprintCallable, Category = "Resource")
    int32 HarvestResource(int32 NodeID, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Resource")
    const FResourceNode* GetNode(int32 NodeID) const;

    UFUNCTION(BlueprintCallable, Category = "Resource")
    TArray<FResourceNode> GetNodesInTerritory(int32 TerritoryID) const;

    UFUNCTION(BlueprintCallable, Category = "Resource")
    void SpawnResourceNode(EWorldResourceType Type, const FVector& Location, int32 TerritoryID, int32 MaxAmount = 1000);

    // Сохранение/загрузка
    UFUNCTION(BlueprintCallable, Category = "Save")
    void SaveResourceState();

    UFUNCTION(BlueprintCallable, Category = "Save")
    void LoadResourceState();

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    bool IsServer() const;
    void RegenerateNodes(float DeltaTime);
    void NotifyMarketplace(int32 NodeID, int32 Harvested);
    void UpdateTerritoryIndex();

    TMap<int32, FResourceNode> ResourceNodes; // серверная копия
    UPROPERTY()
    AResourceWorldState* WorldStateActor;

    TMap<int32, TArray<int32>> TerritoryNodes;
    TMap<int32, float> RegenAccumulators;
    mutable FCriticalSection ResourceMutex;

    int32 NextNodeID = 1;

    UPROPERTY()
    UTerritoryManagerSubsystem* TerritorySystem;

    UPROPERTY()
    UAIWorldDirectorSubsystem* AIDirector;

    static constexpr int32 MAX_RESOURCE_NODES = 5000;
    static constexpr float SPAWN_CHANCE = 0.1f;
};
