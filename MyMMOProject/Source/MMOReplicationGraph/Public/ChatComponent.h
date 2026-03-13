#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChatTypes.h"
#include "ChatMessage.h"
#include "ChatComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CHATSYSTEMCORE_API UChatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UChatComponent();

    // Отправка сообщения (вызывается клиентом)
    UFUNCTION(Server, Reliable)
    void Server_SendChatMessage(const FChatMessage& Message);

    // Получение сообщения (реплицируется клиенту)
    UFUNCTION(Client, Reliable)
    void Client_ReceiveChatMessage(const FChatMessage& Message);

    // Мгновенный мультикаст для локального чата (если нужно)
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_LocalChatMessage(const FChatMessage& Message);

    // Интеграция с системами
    bool IsMuted() const { return bMuted; }
    void SetMuted(bool bMute);

    // Таймеры анти-спама
    void AddMessageCount();

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    bool IsServer() const;

    UPROPERTY(Replicated)
    bool bMuted;

    // Анти-спам (подсчет сообщений за последние 10 секунд)
    TArray<FDateTime> MessageTimestamps;

    static constexpr int32 MAX_MESSAGES_PER_INTERVAL = 5;
    static constexpr int32 INTERVAL_SECONDS = 10;
    static constexpr int32 MUTE_DURATION_SECONDS = 10;

    FTimerHandle UnmuteTimerHandle;
    void ClearSpamCount();
    void OnMuteExpired();
};
