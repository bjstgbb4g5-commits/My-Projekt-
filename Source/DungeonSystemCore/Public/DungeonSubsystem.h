#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DungeonInstance.h"
#include "DungeonTypes.h"
#include "DungeonSubsystem.generated.h"

UCLASS()
class DUNGEONSYSTEMCORE_API UDungeonSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UDungeonSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    int32 CreateDungeonInstance(const TArray<APlayerState*>& Players, EDungeonDifficulty Difficulty);

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    void StartDungeon(int32 InstanceID);

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    void CompleteDungeon(int32 InstanceID);

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    bool CanEnterDungeon(APlayerState* Player, EDungeonDifficulty Difficulty) const;

    // Получение информации о прогрессе для игроков
    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    int32 GetDungeonAttempts(APlayerState* Player, EDungeonDifficulty Difficulty) const;

private:
    bool IsServer() const;
    void CleanupInstances();

    UPROPERTY()
    TMap<int32, FDungeonInstance> ActiveDungeons;

    int32 NextInstanceID = 1;

    // Лимиты: сколько раз игрок может зайти в данж за день/неделю
    TMap<APlayerState*, TMap<EDungeonDifficulty, int32>> DungeonAttempts;

    FTimerHandle CleanupTimer;
    static constexpr float CLEANUP_INTERVAL = 300.f;
};
