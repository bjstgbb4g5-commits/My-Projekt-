#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PersistenceTypes.h"
#include "PlayerSaveManager.generated.h"

UCLASS()
class PERSISTENCESYSTEMCORE_API UPlayerSaveManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void SavePlayer(APlayerState* Player);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    bool LoadPlayer(APlayerState* Player);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void SaveAllPlayers();

    // Загрузка всех игроков обычно не требуется, так как загружаются при логине

private:
    bool IsServer() const;
    FString GetPlayerSaveSlot(const FUniqueNetIdRepl& PlayerId) const;
};
