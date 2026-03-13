#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonAITypes.h"
#include "DungeonDirector.generated.h"

UCLASS()
class DUNGEONAISYSTEMCORE_API UDungeonDirector : public UObject
{
    GENERATED_BODY()

public:
    void AnalyzeGroup();

    const FDungeonGroupProfile& GetGroupProfile() const { return GroupProfile; }

private:
    FDungeonGroupProfile GroupProfile;

    void CollectMetrics();
    void ComputeProfile();
};
