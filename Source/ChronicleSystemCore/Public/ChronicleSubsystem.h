#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "ChronicleTypes.h"
#include "ChronicleSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChronicleStarted, EChronicleID, ChronicleID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeasonStarted, int32, SeasonIndex);

UCLASS()
class CHRONICLESYSTEMCORE_API UChronicleSubsystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UChronicleSubsystem();

    // UWorldSubsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // FTickableGameObject
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

    // Получить текущую хронику
    UFUNCTION(BlueprintCallable, Category = "Chronicle")
    EChronicleID GetCurrentChronicle() const { return ChronicleData.Chronicle; }

    // Получить текущий сезон
    UFUNCTION(BlueprintCallable, Category = "Chronicle")
    int32 GetCurrentSeason() const { return ChronicleData.CurrentSeason; }

    // Получить время окончания текущего сезона
    UFUNCTION(BlueprintCallable, Category = "Chronicle")
    FDateTime GetSeasonEndTime() const;

    // Начать сезон (вызывается автоматически)
    UFUNCTION(BlueprintCallable, Category = "Chronicle")
    void StartSeason(int32 SeasonIndex);

    // Завершить сезон (вызывается автоматически по времени)
    UFUNCTION(BlueprintCallable, Category = "Chronicle")
    void EndSeason();

    // Начать следующую хронику
    UFUNCTION(BlueprintCallable, Category = "Chronicle")
    void StartNextChronicle();

    // Сохранение/загрузка состояния
    UFUNCTION(BlueprintCallable, Category = "Chronicle")
    void SaveChronicleState(const FString& SlotName);

    UFUNCTION(BlueprintCallable, Category = "Chronicle")
    bool LoadChronicleState(const FString& SlotName);

    // События
    UPROPERTY(BlueprintAssignable)
    FOnChronicleStarted OnChronicleStarted;

    UPROPERTY(BlueprintAssignable)
    FOnSeasonStarted OnSeasonStarted;

private:
    bool IsServer() const;
    FDateTime GetUtcNow() const;
    void OnSeasonStart();
    void OnSeasonEnd();
    void OnChronicleEnd();

    UPROPERTY()
    FChronicleData ChronicleData;

    UPROPERTY()
    TArray<FSeasonData> Seasons;

    bool bChronicleTransition = false;
    float SeasonCheckAccumulator = 0.f;
    static constexpr float SEASON_CHECK_INTERVAL = 60.f;
};
