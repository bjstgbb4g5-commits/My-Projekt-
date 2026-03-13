#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldBossTypes.h"
#include "WorldBossSubsystem.generated.h"

UCLASS()
class WORLDBOSSSYSTEMCORE_API UWorldBossSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UWorldBossSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Спавн мирового босса
    UFUNCTION(BlueprintCallable, Category = "WorldBoss")
    void SpawnWorldBoss(int32 BossID, FVector Location);

    // Убить мирового босса
    UFUNCTION(BlueprintCallable, Category = "WorldBoss")
    void KillWorldBoss(int32 BossID);

    // Получить масштабированное здоровье в зависимости от числа игроков
    UFUNCTION(BlueprintCallable, Category = "WorldBoss")
    float GetScaledHealth(int32 BossID, int32 PlayersNearby) const;

    // Проверить, жив ли босс
    UFUNCTION(BlueprintCallable, Category = "WorldBoss")
    bool IsBossAlive(int32 BossID) const;

    // Обновление состояния (проверка респавна)
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

private:
    bool IsServer() const;

    UPROPERTY()
    TMap<int32, FWorldBossData> ActiveBosses;

    static constexpr float RESPAWN_TIME = 86400.f; // 24 часа
};
