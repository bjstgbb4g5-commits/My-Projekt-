#include "OlympiadArenaInstance.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"

AOlympiadArenaInstance::AOlympiadArenaInstance()
{
    bReplicates = true;
    bAlwaysRelevant = true;
    PrimaryActorTick.bCanEverTick = false;
}

void AOlympiadArenaInstance::BeginPlay()
{
    Super::BeginPlay();
}

void AOlympiadArenaInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AOlympiadArenaInstance, CurrentMatchID);
}

void AOlympiadArenaInstance::InitializeArena(int32 MatchID, APlayerController* PlayerA, APlayerController* PlayerB)
{
    if (!HasAuthority()) return;
    CurrentMatchID = MatchID;
    ContestantA = PlayerA;
    ContestantB = PlayerB;
}

void AOlympiadArenaInstance::StartFight()
{
    if (!HasAuthority()) return;
    // Телепортируем игроков на арену, даём 5 секунд на подготовку
    FTimerHandle Dummy;
    GetWorld()->GetTimerManager().SetTimer(FightTimerHandle, this, &AOlympiadArenaInstance::OnFightTimeout, 180.f, false); // 3 минуты боя
}

void AOlympiadArenaInstance::EndFight(APlayerController* Winner)
{
    if (!HasAuthority()) return;
    GetWorld()->GetTimerManager().ClearTimer(FightTimerHandle);
    // Здесь можно вернуть игроков, объявить результат
    // Уведомить OlympiadSubsystem о результате
    Destroy();
}

void AOlympiadArenaInstance::OnFightTimeout()
{
    // По таймауту побеждает тот, у кого больше HP (будет реализовано через проверку)
    // Пока заглушка
    EndFight(nullptr);
}
