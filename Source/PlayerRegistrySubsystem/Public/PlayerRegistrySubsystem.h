#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlayerRegistrySubsystem.generated.h"

UCLASS()
class PLAYERREGISTRYSUBSYSTEM_API UPlayerRegistrySubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "PlayerRegistry")
    void RegisterPlayer(APlayerController* Player);

    UFUNCTION(BlueprintCallable, Category = "PlayerRegistry")
    void UnregisterPlayer(APlayerController* Player);

    UFUNCTION(BlueprintCallable, Category = "PlayerRegistry")
    APlayerController* GetPlayerById(const FUniqueNetIdRepl& PlayerId) const;

    UFUNCTION(BlueprintCallable, Category = "PlayerRegistry")
    TArray<APlayerController*> GetPlayersInRegion(int32 RegionIndex) const; // требует интеграции с WorldGeneratorCore

    UFUNCTION(BlueprintCallable, Category = "PlayerRegistry")
    TArray<APlayerController*> GetAllPlayers() const;

private:
    UPROPERTY()
    TMap<FUniqueNetIdRepl, APlayerController*> PlayerMap;
};
