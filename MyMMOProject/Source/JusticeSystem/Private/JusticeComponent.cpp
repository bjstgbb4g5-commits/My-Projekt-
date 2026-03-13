#include "JusticeComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerState.h"

UJusticeComponent::UJusticeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);

    JusticeState = EJusticeState::None;
    StrikeCount = 0;
}

void UJusticeComponent::BeginPlay()
{
    Super::BeginPlay();
    if (IsServer())
    {
        CheckJusticeTimers();
    }
}

void UJusticeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UJusticeComponent, JusticeState);
    DOREPLIFETIME(UJusticeComponent, StrikeCount);
    DOREPLIFETIME(UJusticeComponent, LawMarkExpiration);
    DOREPLIFETIME(UJusticeComponent, PrisonReleaseTime);
    DOREPLIFETIME(UJusticeComponent, ExecutionDebuffEnd);
}

bool UJusticeComponent::IsServer() const
{
    return GetOwner() && GetOwner()->HasAuthority();
}

FDateTime UJusticeComponent::GetUtcNow() const
{
    return FDateTime::UtcNow();
}

void UJusticeComponent::OnRep_JusticeState()
{
    OnJusticeStateChanged.Broadcast(JusticeState);
}

void UJusticeComponent::RegisterSeriousCrime()
{
    if (!IsServer()) return;

    StrikeCount++;

    if (StrikeCount == 1)
    {
        // Первое нарушение: тюрьма 15 минут + метка на 3 дня
        LawMarkExpiration = GetUtcNow() + FTimespan::FromDays(3);
        Imprison(15);
    }
    else if (StrikeCount == 2)
    {
        // Второе нарушение: тюрьма 45 минут + метка на 5 дней
        LawMarkExpiration = GetUtcNow() + FTimespan::FromDays(5);
        Imprison(45);
    }
    else if (StrikeCount >= 3)
    {
        // Третье нарушение: ожидание казни
        BeginExecutionWaiting();
    }
}

void UJusticeComponent::Imprison(int32 Minutes)
{
    if (!IsServer()) return;

    JusticeState = EJusticeState::Imprisoned;
    PrisonReleaseTime = GetUtcNow() + FTimespan::FromMinutes(Minutes);

    // Телепортация в тюрьму (будет вызываться через GameMode)
}

void UJusticeComponent::BeginExecutionWaiting()
{
    if (!IsServer()) return;

    JusticeState = EJusticeState::AwaitingExecution;
    PrisonReleaseTime = GetUtcNow() + FTimespan::FromMinutes(30); // 30 минут ожидания
}

void UJusticeComponent::ExecutePlayer()
{
    if (!IsServer()) return;

    JusticeState = EJusticeState::ExecutionDebuff;
    ExecutionDebuffEnd = GetUtcNow() + FTimespan::FromHours(24); // дебафф на 24 часа

    StrikeCount = 0;
    LawMarkExpiration = FDateTime();
}

void UJusticeComponent::CheckJusticeTimers()
{
    if (!IsServer()) return;

    FDateTime Now = GetUtcNow();

    if (JusticeState == EJusticeState::Imprisoned && Now >= PrisonReleaseTime)
    {
        // Выход из тюрьмы: если есть метка, переходим в Marked, иначе None
        if (LawMarkExpiration.IsValid() && Now < LawMarkExpiration)
        {
            JusticeState = EJusticeState::Marked;
        }
        else
        {
            JusticeState = EJusticeState::None;
        }
        PrisonReleaseTime = FDateTime();
    }

    if (JusticeState == EJusticeState::AwaitingExecution && Now >= PrisonReleaseTime)
    {
        ExecutePlayer();
    }

    if (JusticeState == EJusticeState::ExecutionDebuff && Now >= ExecutionDebuffEnd)
    {
        JusticeState = EJusticeState::None;
        ExecutionDebuffEnd = FDateTime();
    }

    if (JusticeState == EJusticeState::Marked && LawMarkExpiration.IsValid() && Now >= LawMarkExpiration)
    {
        LawMarkExpiration = FDateTime();
        JusticeState = EJusticeState::None;
    }
}

float UJusticeComponent::GetMovementSpeedMultiplier() const
{
    return (JusticeState == EJusticeState::ExecutionDebuff) ? 0.5f : 1.0f;
}

float UJusticeComponent::GetDamageOutputMultiplier() const
{
    return (JusticeState == EJusticeState::ExecutionDebuff) ? 0.7f : 1.0f;
}

float UJusticeComponent::GetDamageTakenMultiplier() const
{
    return (JusticeState == EJusticeState::ExecutionDebuff) ? 1.2f : 1.0f;
}

bool UJusticeComponent::ShouldGuardsAttack() const
{
    return (JusticeState == EJusticeState::Marked ||
            JusticeState == EJusticeState::AwaitingExecution ||
            JusticeState == EJusticeState::ExecutionDebuff);
}

void UJusticeComponent::ResetAllControlState()
{
    if (!IsServer()) return;
    JusticeState = EJusticeState::None;
    StrikeCount = 0;
    LawMarkExpiration = FDateTime();
    PrisonReleaseTime = FDateTime();
    ExecutionDebuffEnd = FDateTime();
}
