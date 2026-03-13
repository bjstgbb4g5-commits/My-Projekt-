#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "OlympiadTypes.h"
#include "OlympiadSubsystem.generated.h"

class UOlympiadSaveGame;
class AOlympiadArenaInstance;

UCLASS()
class OLYMPIADSYSTEMCORE_API UOlympiadSubsystem
    : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UOlympiadSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

    // Регистрация игрока на Олимпиаду
    UFUNCTION(BlueprintCallable, Category = "Olympiad")
    void RegisterPlayer(APlayerState* Player);

    // Снятие с регистрации
    UFUNCTION(BlueprintCallable, Category = "Olympiad")
    void UnregisterPlayer(APlayerState* Player);

    // Создать матч между двумя игроками
    UFUNCTION(BlueprintCallable, Category = "Olympiad")
    int32 CreateMatch(const FUniqueNetIdRepl& PlayerA, const FUniqueNetIdRepl& PlayerB);

    // Завершить матч (вызывается из арены)
    UFUNCTION(BlueprintCallable, Category = "Olympiad")
    void FinishMatch(int32 MatchID, const FUniqueNetIdRepl& Winner);

    // Получить топ игроков
    UFUNCTION(BlueprintCallable, Category = "Olympiad")
    TArray<FOlympiadPlayerData> GetTopPlayers(int32 Count) const;

    // Текущее состояние Олимпиады
    UFUNCTION(BlueprintCallable, Category = "Olympiad")
    EOlympiadState GetCurrentState() const { return CurrentState; }

    UFUNCTION(BlueprintCallable, Category = "Olympiad")
    FDateTime GetSeasonEndTime() const { return SeasonEndTime; }

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    bool IsServer() const;
    void TryMatchmaking();
    void StartOlympiadWeek();
    void EndOlympiadWeek();
    void GiveWeeklyRewards();
    void RecordChampionsInChronicle();
    void SaveState();
    void LoadState();

    UPROPERTY(Replicated)
    EOlympiadState CurrentState;

    UPROPERTY(Replicated)
    FDateTime RegistrationStart;

    UPROPERTY(Replicated)
    FDateTime ActiveStart;

    UPROPERTY(Replicated)
    FDateTime FinalStart;

    UPROPERTY(Replicated)
    FDateTime SeasonEndTime;

    UPROPERTY(Replicated)
    TMap<FUniqueNetIdRepl, FOlympiadPlayerData> PlayerRatings; // реплицируется для отображения

    UPROPERTY()
    TMap<int32, FOlympiadMatch> ActiveMatches;

    UPROPERTY()
    TSet<FUniqueNetIdRepl> PlayersInMatch; // для избежания повторных матчей

    UPROPERTY()
    TMap<FUniqueNetIdRepl, float> LastMatchTime; // анти‑спам

    int32 NextMatchID = 1;

    float MatchmakingTimer = 0.f;
    static constexpr float MATCHMAKING_INTERVAL = 5.f;
    static constexpr int32 MAX_ACTIVE_MATCHES = 20;
    static constexpr int32 REGISTRATION_DAYS = 1;
    static constexpr int32 ACTIVE_DAYS = 5;
    static constexpr int32 FINAL_DAYS = 1;

    UPROPERTY()
    class UChronicleSubsystem* Chronicle;

    UPROPERTY()
    UOlympiadSaveGame* SaveGame;

    FTimerHandle SaveTimerHandle;
    void AutoSave();
};
