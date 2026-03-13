#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RaidInstance.h"
#include "RaidTypes.h"
#include "RaidSubsystem.generated.h"

UCLASS()
class RAIDSYSTEMCORE_API URaidSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    URaidSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Создать новый рейд
    UFUNCTION(BlueprintCallable, Category = "Raid")
    int32 CreateRaid(const TArray<APlayerState*>& Players, ERaidDifficulty Difficulty);

    // Начать рейд
    UFUNCTION(BlueprintCallable, Category = "Raid")
    void StartRaid(int32 RaidID);

    // Уведомить о смерти босса
    UFUNCTION(BlueprintCallable, Category = "Raid")
    void BossKilled(int32 RaidID);

    // Получить информацию о рейде
    UFUNCTION(BlueprintCallable, Category = "Raid")
    FRaidInstance GetRaidInfo(int32 RaidID) const;

    // Получить все активные рейды
    UFUNCTION(BlueprintCallable, Category = "Raid")
    TArray<int32> GetActiveRaidIDs() const;

private:
    bool IsServer() const;
    void CleanupCompletedRaids();

    UPROPERTY()
    TMap<int32, FRaidInstance> ActiveRaids;

    int32 NextRaidID = 1;

    FTimerHandle CleanupTimer;
    static constexpr float CLEANUP_INTERVAL = 300.f; // 5 минут
};
