#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameFramework/SaveGame.h"
#include "BanManager.generated.h"

USTRUCT()
struct FBanRecord
{
    GENERATED_BODY()

    UPROPERTY()
    FString PlayerID;

    UPROPERTY()
    FDateTime BanTime;

    UPROPERTY()
    FString Reason;
};

UCLASS()
class ANTICHEATSYSTEMCORE_API UBanSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<FBanRecord> BannedPlayers;
};

UCLASS()
class ANTICHEATSYSTEMCORE_API UBanManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Justice")
    bool IsPlayerBanned(const FUniqueNetIdRepl& PlayerID) const;

    UFUNCTION(BlueprintCallable, Category = "Justice")
    void BanPlayer(const FUniqueNetIdRepl& PlayerID, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Justice")
    void KickPlayer(const FUniqueNetIdRepl& PlayerID, const FString& Reason);

protected:
    UPROPERTY()
    UBanSaveGame* SaveGame;

    void LoadBanList();
    void SaveBanList();
};
