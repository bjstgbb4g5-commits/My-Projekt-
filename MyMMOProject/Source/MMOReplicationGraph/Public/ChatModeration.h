#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChatModeration.generated.h"

UCLASS()
class CHATSYSTEMCORE_API UChatModeration : public UObject
{
    GENERATED_BODY()

public:
    // Фильтр мата (замена)
    static FString FilterProfanity(const FString& Input);

    // Проверка на мут
    static bool IsMuted(APlayerController* Player);

    // Установить мут (вызывается из подсистемы)
    static void SetMute(APlayerController* Player, bool bMute, float DurationSeconds = 0.f);
};
