#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChatRateLimiter.generated.h"

UCLASS()
class CHATSYSTEMCORE_API UChatRateLimiter : public UObject
{
    GENERATED_BODY()

public:
    bool CheckMessage(APlayerController* Player, FString& OutErrorMessage);
    void ReportMessage(APlayerController* Player);

private:
    struct FPlayerRateData
    {
        TArray<FDateTime> Timestamps;
        FTimerHandle UnmuteTimer;
    };

    TMap<FUniqueNetIdRepl, FPlayerRateData> RateData;

    static constexpr int32 MAX_MESSAGES = 5;
    static constexpr int32 INTERVAL_SECONDS = 10;
    static constexpr int32 MUTE_SECONDS = 10;

    void MutePlayer(const FUniqueNetIdRepl& PlayerId);
    void UnmutePlayer(const FUniqueNetIdRepl& PlayerId);
};
