#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ChatTypes.h"
#include "ChatMessage.h"
#include "ChatSubsystem.generated.h"

class UChatComponent;
class UPlayerRegistrySubsystem;
class UPartySubsystem;
class UGuildSubsystem;

UCLASS()
class CHATSYSTEMCORE_API UChatSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Основная функция маршрутизации
    void RouteMessage(AActor* Sender, const FChatMessage& Message);

    // Отправка системного сообщения всем
    void BroadcastSystemMessage(const FString& Text);

private:
    bool IsServer() const;

    UPROPERTY()
    UPlayerRegistrySubsystem* PlayerRegistry;

    UPROPERTY()
    UPartySubsystem* PartySubsystem;

    UPROPERTY()
    UGuildSubsystem* GuildSubsystem;

    // Вспомогательные функции для каждого канала
    void SendLocal(AActor* Sender, const FChatMessage& Message);
    void SendParty(AActor* Sender, const FChatMessage& Message);
    void SendGuild(AActor* Sender, const FChatMessage& Message);
    void SendTrade(const FChatMessage& Message);
    void SendGlobal(const FChatMessage& Message);
    void SendWhisper(AActor* Sender, const FChatMessage& Message);
    void SendSystem(const FChatMessage& Message); // только от сервера

    // Получить компонент чата у игрока
    UChatComponent* GetChatComponent(APlayerController* PC) const;
};
