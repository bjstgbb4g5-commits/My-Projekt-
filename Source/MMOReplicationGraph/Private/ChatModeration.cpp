#include "ChatModeration.h"
#include "ChatComponent.h"

FString UChatModeration::FilterProfanity(const FString& Input)
{
    // Простейший фильтр: можно заменить по словарю
    // Для примера заменим "badword" на "***"
    FString Result = Input;
    Result.ReplaceInline(TEXT("badword"), TEXT("***"), ESearchCase::IgnoreCase);
    return Result;
}

bool UChatModeration::IsMuted(APlayerController* Player)
{
    if (!Player) return false;
    UChatComponent* Comp = Player->GetPawn() ? Player->GetPawn()->FindComponentByClass<UChatComponent>() : nullptr;
    return Comp ? Comp->IsMuted() : false;
}

void UChatModeration::SetMute(APlayerController* Player, bool bMute, float DurationSeconds)
{
    if (!Player) return;
    UChatComponent* Comp = Player->GetPawn() ? Player->GetPawn()->FindComponentByClass<UChatComponent>() : nullptr;
    if (Comp)
    {
        Comp->SetMuted(bMute);
        // DurationSeconds можно игнорировать, если таймер уже есть в компоненте
    }
}
