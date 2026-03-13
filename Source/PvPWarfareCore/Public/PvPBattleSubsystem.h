#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "PvPTypes.h"
#include "PvPBattleSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleStarted, int32, BattleID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBattleFinished, int32, BattleID, EPvPBattleResult, Result);

UCLASS()
class PVPWARFARECORE_API UPvPBattleSubsystem
    : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    // Создать новый бой
    UFUNCTION(BlueprintCallable, Category = "PvP")
    int32 CreateBattle(EPvPBattleType Type, int32 MaxAttackers, int32 MaxDefenders, float DurationSeconds);

    // Присоединиться к бою
    UFUNCTION(BlueprintCallable, Category = "PvP")
    bool JoinBattle(int32 BattleID, APlayerController* Player, bool bAsAttacker);

    // Начать бой
    UFUNCTION(BlueprintCallable, Category = "PvP")
    void StartBattle(int32 BattleID);

    // Завершить бой с результатом
    UFUNCTION(BlueprintCallable, Category = "PvP")
    void FinishBattle(int32 BattleID, EPvPBattleResult Result);

    // Получить информацию о бое
    UFUNCTION(BlueprintCallable, Category = "PvP")
    FPvPBattle GetBattleInfo(int32 BattleID) const;

    // События
    UPROPERTY(BlueprintAssignable)
    FOnBattleStarted OnBattleStarted;

    UPROPERTY(BlueprintAssignable)
    FOnBattleFinished OnBattleFinished;

    // FTickableGameObject
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
    bool IsServer() const;

    UPROPERTY()
    TMap<int32, FPvPBattle> ActiveBattles;

    UPROPERTY()
    TArray<FPvPBattleLog> BattleLogs;

    int32 NextBattleID = 1;
    static constexpr int32 MAX_LOGS = 500;
    static constexpr float REJOIN_COOLDOWN = 10.f;

    TMap<APlayerState*, float> LastBattleLeave; // анти‑эксплойт
};
