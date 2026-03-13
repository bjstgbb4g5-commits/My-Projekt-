#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "HouseTypes.h"
#include "HouseSubsystem.generated.h"

class UGuildSubsystem;
class UPlayerRegistrySubsystem;
class UTerritoryManagerSubsystem;

DECLARE_LOG_CATEGORY_EXTERN(LogHouseSystem, Log, All);

UCLASS()
class HOUSESYSTEMCORE_API UHouseSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UHouseSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // House management
    UFUNCTION(BlueprintCallable, Category = "House")
    int32 CreateHouse(const FString& HouseName, EHouseType HouseType, APlayerState* Founder, int32 GuildId);

    UFUNCTION(BlueprintCallable, Category = "House")
    bool InvitePlayer(int32 HouseId, APlayerState* Inviter, APlayerState* Target);

    UFUNCTION(BlueprintCallable, Category = "House")
    bool AcceptInvite(int32 HouseId, APlayerState* Player);

    UFUNCTION(BlueprintCallable, Category = "House")
    bool DeclineInvite(int32 HouseId, APlayerState* Player);

    UFUNCTION(BlueprintCallable, Category = "House")
    bool LeaveHouse(APlayerState* Player);

    UFUNCTION(BlueprintCallable, Category = "House")
    bool KickMember(int32 HouseId, APlayerState* Requester, APlayerState* Target);

    UFUNCTION(BlueprintCallable, Category = "House")
    bool UpgradeHouse(int32 HouseId, APlayerState* Requester);

    UFUNCTION(BlueprintCallable, Category = "House")
    bool DestroyHouse(int32 HouseId, APlayerState* Requester);

    UFUNCTION(BlueprintCallable, Category = "House")
    bool CaptureTerritory(int32 HouseId, int32 TerritoryId, APlayerState* Requester);

    // Queries
    UFUNCTION(BlueprintCallable, Category = "House")
    const FHouseData* GetHouse(int32 HouseId) const;

    UFUNCTION(BlueprintCallable, Category = "House")
    int32 GetPlayerHouseId(APlayerState* Player) const;

    UFUNCTION(BlueprintCallable, Category = "House")
    TArray<FHouseData> GetGuildHouses(int32 GuildId) const;

    // Save/load
    void SaveHouses();
    void LoadHouses();

private:
    bool IsServer() const;

    // Guild rank helper (добавим, хотя предполагаем, что есть в GuildSubsystem)
    EGuildRank GetPlayerGuildRank(APlayerState* Player) const;

    // Прямой метод для JoinHouse (используется только через приглашения)
    bool JoinHouse(int32 HouseId, APlayerState* Player, EHouseRank InitialRank = EHouseRank::Recruit);

    UPROPERTY()
    TMap<int32, FHouseData> Houses;

    // Карта для быстрого поиска игроков в домах
    TMap<FUniqueNetIdRepl, int32> PlayerHouseMap;

    // Карта приглашений: HouseId -> список приглашенных игроков
    TMap<int32, TArray<FUniqueNetIdRepl>> PendingInvites;

    int32 NextHouseId = 1;

    UPROPERTY()
    UGuildSubsystem* GuildSubsystem;

    UPROPERTY()
    UPlayerRegistrySubsystem* PlayerRegistry;

    UPROPERTY()
    UTerritoryManagerSubsystem* TerritorySubsystem;

    // Константы
    static constexpr int32 MAX_HOUSE_NAME_LENGTH = 50;
    static constexpr int32 MAX_HOUSES = 5000;
    static constexpr int32 MAX_MEMBERS_PER_HOUSE = 200;
    static constexpr int32 MAX_TERRITORIES_PER_HOUSE = 20;
    static constexpr int32 MAX_HOUSE_ID = 100000000;
};
