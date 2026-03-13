#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "TerritoryTypes.h"
#include "TerritoryManagerSubsystem.generated.h"

class UGuildSubsystem;

UCLASS()
class WORLDTERRITORYSYSTEMCORE_API UTerritoryManagerSubsystem
    : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UTerritoryManagerSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

    // Territory API
    UFUNCTION(BlueprintCallable, Category = "Territory")
    bool CaptureTerritory(int32 TerritoryID, const FName& GuildID);

    UFUNCTION(BlueprintCallable, Category = "Territory")
    bool SetTaxRate(int32 TerritoryID, float NewTax);

    UFUNCTION(BlueprintCallable, Category = "Territory")
    FTerritoryData* GetTerritory(int32 TerritoryID);

    UFUNCTION(BlueprintCallable, Category = "Territory")
    TArray<FTerritoryData> GetGuildTerritories(const FName& GuildID) const;

    UFUNCTION(BlueprintCallable, Category = "Territory")
    TArray<FTerritoryData> GetAllTerritories() const;

    // Сохранение
    UFUNCTION(BlueprintCallable, Category = "Territory")
    void SaveTerritories();

    UFUNCTION(BlueprintCallable, Category = "Territory")
    void LoadTerritories();

private:
    bool IsServer() const;
    void RegenerateResources(float DeltaTime);

    UPROPERTY()
    TMap<int32, FTerritoryData> Territories;

    UPROPERTY()
    UGuildSubsystem* GuildSystem;

    static constexpr int32 MAX_TERRITORIES = 500;
    static constexpr float RESOURCE_REGEN_INTERVAL = 60.f;
    float ResourceRegenTimer = 0.f;
};
