#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "ApostleTypes.h"
#include "ApostleSubsystem.generated.h"

UCLASS()
class APOSTLESYSTEMCORE_API UApostleSubsystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UApostleSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Заспавнить апостола
    UFUNCTION(BlueprintCallable, Category = "Apostle")
    void SpawnApostle(EApostleType Apostle, FVector Location);

    // Убить апостола
    UFUNCTION(BlueprintCallable, Category = "Apostle")
    void KillApostle(EApostleType Apostle);

    // Проверить, жив ли апостол
    UFUNCTION(BlueprintCallable, Category = "Apostle")
    bool IsApostleAlive(EApostleType Apostle) const;

    // Получить текущее состояние апостола
    UFUNCTION(BlueprintCallable, Category = "Apostle")
    EApostleState GetApostleState(EApostleType Apostle) const;

    // FTickableGameObject
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

private:
    bool IsServer() const;

    UPROPERTY()
    TMap<EApostleType, FApostleData> ActiveApostles;
};
