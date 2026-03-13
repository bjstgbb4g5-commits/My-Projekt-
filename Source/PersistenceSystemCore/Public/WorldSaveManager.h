#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldSaveManager.generated.h"

UCLASS()
class PERSISTENCESYSTEMCORE_API UWorldSaveManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void SaveWorld();

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    bool LoadWorld();

private:
    bool IsServer() const;
    // Можно добавить структуру для сохранения мира
};
