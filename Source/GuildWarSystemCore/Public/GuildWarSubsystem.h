#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "GuildWarTypes.h"
#include "GuildWarSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGuildWarSystem, Log, All);

UCLASS()
class GUILDWARSYSTEMCORE_API UGuildWarSubsystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UGuildWarSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // FTickableGameObject
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

    // War management
    UFUNCTION(BlueprintCallable, Category = "GuildWar")
    int32 DeclareWar(int32 AttackerGuildId, int32 DefenderGuildId);

    UFUNCTION(BlueprintCallable, Category = "GuildWar")
    bool AcceptWar(int32 WarId);

    UFUNCTION(BlueprintCallable, Category = "GuildWar")
    bool CancelWar(int32 WarId);

    UFUNCTION(BlueprintCallable, Category = "GuildWar")
    bool AddWarScore(int32 WarId, int32 GuildId, int32 Score);

    UFUNCTION(BlueprintCallable, Category = "GuildWar")
    bool FinishWar(int32 WarId);

    // Queries
    UFUNCTION(BlueprintCallable, Category = "GuildWar")
    bool IsGuildAtWar(int32 GuildId) const;

    UFUNCTION(BlueprintCallable, Category = "GuildWar")
    int32 GetCurrentWarIdForGuild(int32 GuildId) const;

    UFUNCTION(BlueprintCallable, Category = "GuildWar")
    TArray<FGuildWar> GetGuildWars(int32 GuildId) const;

    // Territory integration (placeholder)
    UFUNCTION(BlueprintCallable, Category = "GuildWar")
    void CaptureTerritoryForGuild(int32 WinnerGuildId, int32 LoserGuildId);

private:
    bool IsServer() const;
    void CleanupOldHistory();

    UPROPERTY()
    TMap<int32, FGuildWar> ActiveWars;          // WarId -> War (active or declared)

    UPROPERTY()
    TMap<int32, FGuildWar> FinishedWarHistory;  // WarId -> War (finished/cancelled)

    TMap<int32, int32> GuildWarMap;               // GuildId -> WarId (active wars only)
    TMap<int32, FDateTime> LastWarDeclaration;    // GuildId -> last declaration time

    int32 NextWarId = 1;
    float TickTimer = 0.f;

    static constexpr int32 DEFAULT_WAR_DURATION_HOURS = 24;
    static constexpr float TICK_INTERVAL = 2.0f;
    static constexpr int32 WAR_HISTORY_HOURS = 48;
    static constexpr int32 WAR_DECLARATION_COOLDOWN_HOURS = 1;
    static constexpr int32 MAX_WAR_ID = 100000000;
    static constexpr int32 MAX_ACTIVE_WARS = 100000;
};
