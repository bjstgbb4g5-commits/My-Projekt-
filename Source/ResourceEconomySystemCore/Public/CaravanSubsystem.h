#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "CaravanTypes.h"
#include "CaravanSubsystem.generated.h"

class ACaravanActor;
class UResourceNodeSubsystem;
class UMarketplaceSubsystem;

UCLASS()
class CARAVANSYSTEMCORE_API UCaravanSubsystem
    : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UCaravanSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

    // Создать караван (вызывается гильдией)
    UFUNCTION(BlueprintCallable, Category = "Caravan")
    int32 CreateCaravan(FName GuildID, int32 OriginCity, int32 DestinationCity, const TArray<FCaravanCargo>& Cargo);

    // Атаковать караван (вызывается из PvPWarfareCore)
    UFUNCTION(BlueprintCallable, Category = "Caravan")
    bool AttackCaravan(int32 CaravanID);

    // Уничтожить караван (при потере HP)
    UFUNCTION(BlueprintCallable, Category = "Caravan")
    bool DestroyCaravan(int32 CaravanID);

    // Завершить караван (доставка)
    UFUNCTION(BlueprintCallable, Category = "Caravan")
    bool DeliverCaravan(int32 CaravanID);

    // Получить информацию о караване
    UFUNCTION(BlueprintCallable, Category = "Caravan")
    const FCaravanData* GetCaravanData(int32 CaravanID) const;

    // Все активные караваны (для отладки)
    UFUNCTION(BlueprintCallable, Category = "Caravan")
    TArray<int32> GetActiveCaravanIDs() const;

    static constexpr int32 MAX_ACTIVE_CARAVANS = 200;

private:
    bool IsServer() const;
    void UpdateCaravans(float DeltaTime);
    void SpawnLoot(const FCaravanData& Caravan, const FVector& Location);
    void TransferResourcesToDestination(const FCaravanData& Caravan);

    UPROPERTY()
    TMap<int32, TWeakObjectPtr<ACaravanActor>> ActiveCaravans; // слабые указатели

    int32 NextCaravanID = 1;
    static constexpr float CARAVAN_SPEED = 0.01f; // прогресс в секунду
};
