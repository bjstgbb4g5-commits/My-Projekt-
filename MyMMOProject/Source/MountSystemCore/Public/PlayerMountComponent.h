#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MountData.h"
#include "PlayerMountComponent.generated.h"

class AMountActor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MOUNTSYSTEMCORE_API UPlayerMountComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPlayerMountComponent();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category = "Mount")
    void SummonMount(int32 MountID);

    UFUNCTION(BlueprintCallable, Category = "Mount")
    void DespawnMount();

    UFUNCTION(BlueprintCallable, Category = "Mount")
    void AddMount(const FMountData& Mount);

    UFUNCTION(BlueprintCallable, Category = "Mount")
    void RemoveMount(int32 MountID);

    UFUNCTION(BlueprintCallable, Category = "Mount")
    FMountData GetMountData(int32 MountID) const;

    UFUNCTION(BlueprintCallable, Category = "Mount")
    bool OwnsMount(int32 MountID) const;

    UFUNCTION(BlueprintCallable, Category = "Mount")
    TArray<FMountData> GetAllMounts() const { return OwnedMounts; }

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Mount")
    TArray<FMountData> OwnedMounts;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Mount")
    int32 ActiveMountID = -1;

private:
    bool IsServer() const;
    UPROPERTY()
    AMountActor* SpawnedMountActor;
};
