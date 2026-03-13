#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "QueueTypes.h"
#include "MatchInstance.h"
#include "MatchmakingSubsystem.generated.h"

class UQueueManager;
class UMatchmakingComponent;

UCLASS()
class MATCHMAKINGSYSTEMCORE_API UMatchmakingSubsystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UMatchmakingSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // FTickableGameObject
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

    // Добавить игрока в очередь (вызывается извне)
    UFUNCTION(BlueprintCallable, Category = "Matchmaking")
    bool JoinQueue(APlayerController* Player, EMatchQueueType QueueType, EPlayerRole Role = EPlayerRole::Any);

    // Покинуть очередь
    UFUNCTION(BlueprintCallable, Category = "Matchmaking")
    bool LeaveQueue(APlayerController* Player);

private:
    bool IsServer() const;

    UPROPERTY()
    UQueueManager* QueueManager;

    TArray<FMatchInstance> ActiveMatches;
    int32 NextMatchId = 1;

    // Функции создания матчей
    void ProcessArena1v1();
    void ProcessArena3v3();
    void ProcessBattleground();
    void ProcessDungeon();
    void ProcessRaid();

    // Уведомить игроков о матче
    void NotifyMatchFound(const FMatchInstance& Match);

    // Получить компонент игрока
    UMatchmakingComponent* GetMatchmakingComponent(APlayerController* Player) const;

    // Проверка ролей для данжей
    bool CheckDungeonRoles(const TArray<FPlayerQueueData>& Players) const;
};
