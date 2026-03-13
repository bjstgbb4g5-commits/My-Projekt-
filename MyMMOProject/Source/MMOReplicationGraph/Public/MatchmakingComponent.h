#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QueueTypes.h"
#include "MatchmakingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchFound, int32, MatchId);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MATCHMAKINGSYSTEMCORE_API UMatchmakingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMatchmakingComponent();

    // Уведомление от сервера о найденном матче
    UFUNCTION(Client, Reliable)
    void Client_MatchFound(int32 MatchId, EMatchQueueType QueueType, const TArray<FUniqueNetIdRepl>& TeamPlayers);

    // Событие для Blueprint/UI
    UPROPERTY(BlueprintAssignable, Category = "Matchmaking")
    FOnMatchFound OnMatchFound;

    // Текущее состояние
    bool IsInQueue() const { return bInQueue; }
    EMatchQueueType GetCurrentQueue() const { return CurrentQueue; }

    // Вызывается с сервера при добавлении в очередь
    void SetQueueState(bool bQueued, EMatchQueueType QueueType);

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY(Replicated)
    bool bInQueue;

    UPROPERTY(Replicated)
    EMatchQueueType CurrentQueue;
};
