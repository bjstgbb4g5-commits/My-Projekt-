#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "PvETypes.h"
#include "NoxEventSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoxDefeated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoxSpawned);

UCLASS()
class PVEWORLDCORE_API UNoxEventSubsystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UNoxEventSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Запустить финальное событие Нокса (когда условия выполнены)
    UFUNCTION(BlueprintCallable, Category = "Nox")
    void StartNoxEvent();

    // Завершить событие (Нокс побеждён)
    UFUNCTION(BlueprintCallable, Category = "Nox")
    void CompleteNoxEvent();

    // Проверить, активно ли событие
    UFUNCTION(BlueprintCallable, Category = "Nox")
    bool IsNoxEventActive() const { return bNoxActive; }

    // События
    UPROPERTY(BlueprintAssignable)
    FOnNoxSpawned OnNoxSpawned;

    UPROPERTY(BlueprintAssignable)
    FOnNoxDefeated OnNoxDefeated;

    // FTickableGameObject
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

private:
    bool IsServer() const;
    void SpawnNox();
    void SpawnFragment(const FVector& Location);

    UPROPERTY()
    class ANoxBossCharacter* NoxBoss;

    UPROPERTY()
    class UNoxFragmentSubsystem* FragmentSubsystem;

    bool bNoxActive;
    FTimerHandle FragmentSpawnTimerHandle;
};
