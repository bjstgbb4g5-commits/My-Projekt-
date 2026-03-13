#include "OlympiadSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "OlympiadSaveGame.h"
#include "ChronicleSystemCore/Public/ChronicleSubsystem.h"
#include "OlympiadArenaInstance.h"

UOlympiadSubsystem::UOlympiadSubsystem()
{
}

void UOlympiadSubsystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UOlympiadSubsystem, CurrentState);
    DOREPLIFETIME(UOlympiadSubsystem, RegistrationStart);
    DOREPLIFETIME(UOlympiadSubsystem, ActiveStart);
    DOREPLIFETIME(UOlympiadSubsystem, FinalStart);
    DOREPLIFETIME(UOlympiadSubsystem, SeasonEndTime);
    DOREPLIFETIME(UOlympiadSubsystem, PlayerRatings);
}

void UOlympiadSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        Chronicle = GetWorld()->GetSubsystem<UChronicleSubsystem>();
        LoadState();
        StartOlympiadWeek(); // или восстановить состояние
        GetWorld()->GetTimerManager().SetTimer(SaveTimerHandle, this, &UOlympiadSubsystem::AutoSave, 300.0f, true); // автосохранение каждые 5 минут
    }
}

void UOlympiadSubsystem::Deinitialize()
{
    if (IsServer())
    {
        GetWorld()->GetTimerManager().ClearTimer(SaveTimerHandle);
        SaveState();
    }
    PlayerRatings.Empty();
    ActiveMatches.Empty();
    PlayersInMatch.Empty();
    LastMatchTime.Empty();
    Super::Deinitialize();
}

bool UOlympiadSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UOlympiadSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UOlympiadSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UOlympiadSubsystem, STATGROUP_Tickables);
}

void UOlympiadSubsystem::Tick(float DeltaTime)
{
    // Обновление состояния сезона (по времени)
    FDateTime Now = FDateTime::UtcNow();

    switch (CurrentState)
    {
    case EOlympiadState::Registration:
        if (Now >= ActiveStart)
            CurrentState = EOlympiadState::Active;
        break;
    case EOlympiadState::Active:
        if (Now >= FinalStart)
        {
            CurrentState = EOlympiadState::Finished;
            EndOlympiadWeek();
        }
        else
        {
            MatchmakingTimer += DeltaTime;
            if (MatchmakingTimer >= MATCHMAKING_INTERVAL)
            {
                MatchmakingTimer = 0.f;
                TryMatchmaking();
            }
        }
        break;
    case EOlympiadState::Finished:
        if (Now >= RegistrationStart + FTimespan::FromDays(7))
            StartOlympiadWeek();
        break;
    default:
        break;
    }
}

void UOlympiadSubsystem::StartOlympiadWeek()
{
    FDateTime Now = FDateTime::UtcNow();
    // Начало регистрации – ближайший понедельник 00:00 (упрощённо)
    RegistrationStart = Now;
    ActiveStart = RegistrationStart + FTimespan::FromDays(REGISTRATION_DAYS);
    FinalStart = ActiveStart + FTimespan::FromDays(ACTIVE_DAYS);
    SeasonEndTime = FinalStart + FTimespan::FromDays(FINAL_DAYS);

    CurrentState = EOlympiadState::Registration;
    PlayerRatings.Empty(); // можно сохранять историю, но для простоты сброс
    ActiveMatches.Empty();
    PlayersInMatch.Empty();
    NextMatchID = 1;

    UE_LOG(LogTemp, Log, TEXT("Olympiad week started. Registration phase until %s"), *ActiveStart.ToString());
}

void UOlympiadSubsystem::EndOlympiadWeek()
{
    GiveWeeklyRewards();
    RecordChampionsInChronicle();
    UE_LOG(LogTemp, Log, TEXT("Olympiad week ended."));
}

void UOlympiadSubsystem::RegisterPlayer(APlayerState* Player)
{
    if (!IsServer() || !Player) return;
    if (CurrentState != EOlympiadState::Registration) return;

    FUniqueNetIdRepl PlayerId = Player->GetUniqueId();
    if (!PlayerId.IsValid()) return;

    if (PlayerRatings.Contains(PlayerId)) return;

    FOlympiadPlayerData Data;
    Data.PlayerID = PlayerId;
    Data.Rating = 1000;
    Data.Wins = 0;
    Data.Losses = 0;
    Data.WeeklyPoints = 0;
    PlayerRatings.Add(PlayerId, Data);
}

void UOlympiadSubsystem::UnregisterPlayer(APlayerState* Player)
{
    if (!IsServer() || !Player) return;
    FUniqueNetIdRepl PlayerId = Player->GetUniqueId();
    if (!PlayerId.IsValid()) return;
    PlayerRatings.Remove(PlayerId);
    PlayersInMatch.Remove(PlayerId);
    LastMatchTime.Remove(PlayerId);
}

int32 UOlympiadSubsystem::CreateMatch(const FUniqueNetIdRepl& PlayerA, const FUniqueNetIdRepl& PlayerB)
{
    if (!IsServer()) return -1;
    if (!PlayerA.IsValid() || !PlayerB.IsValid()) return -1;

    // Проверка, не заняты ли игроки
    if (PlayersInMatch.Contains(PlayerA) || PlayersInMatch.Contains(PlayerB))
        return -1;

    // Проверка кулдауна (не чаще 1 раза в минуту)
    float Now = GetWorld()->GetTimeSeconds();
    float* LastA = LastMatchTime.Find(PlayerA);
    if (LastA && Now - *LastA < 60.f) return -1;
    float* LastB = LastMatchTime.Find(PlayerB);
    if (LastB && Now - *LastB < 60.f) return -1;

    // Создаём экземпляр арены
    FActorSpawnParameters SpawnParams;
    AOlympiadArenaInstance* Arena = GetWorld()->SpawnActor<AOlympiadArenaInstance>(AOlympiadArenaInstance::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    if (!Arena) return -1;

    int32 MatchID = NextMatchID++;
    Arena->InitializeArena(MatchID, nullptr, nullptr); // здесь нужно передать контроллеры игроков
    // В реальности нужно найти APlayerController по PlayerId

    FOlympiadMatch Match;
    Match.MatchID = MatchID;
    Match.PlayerA = PlayerA;
    Match.PlayerB = PlayerB;
    Match.State = EOlympiadMatchState::Fighting; // или Waiting, если нужно время на подготовку
    Match.StartTime = FDateTime::UtcNow();
    ActiveMatches.Add(MatchID, Match);

    PlayersInMatch.Add(PlayerA);
    PlayersInMatch.Add(PlayerB);
    LastMatchTime.Add(PlayerA, Now);
    LastMatchTime.Add(PlayerB, Now);

    return MatchID;
}

void UOlympiadSubsystem::FinishMatch(int32 MatchID, const FUniqueNetIdRepl& Winner)
{
    FOlympiadMatch* Match = ActiveMatches.Find(MatchID);
    if (!Match) return;
    if (!Winner.IsValid()) return; // если таймаут, можно выбрать по оставшемуся здоровью

    FUniqueNetIdRepl Loser = (Winner == Match->PlayerA) ? Match->PlayerB : Match->PlayerA;

    // Обновление статистики
    if (PlayerRatings.Contains(Winner))
    {
        PlayerRatings[Winner].Wins++;
        PlayerRatings[Winner].WeeklyPoints += 10;
    }
    if (PlayerRatings.Contains(Loser))
    {
        PlayerRatings[Loser].Losses++;
    }

    // ELO расчёт
    if (PlayerRatings.Contains(Winner) && PlayerRatings.Contains(Loser))
    {
        int32 Rw = PlayerRatings[Winner].Rating;
        int32 Rl = PlayerRatings[Loser].Rating;
        float Ew = 1.f / (1.f + FMath::Pow(10.f, (Rl - Rw) / 400.f));
        int32 K = 32;
        PlayerRatings[Winner].Rating += K * (1 - Ew);
        PlayerRatings[Loser].Rating += K * (0 - (1 - Ew));
    }

    // Освобождаем игроков
    PlayersInMatch.Remove(Match->PlayerA);
    PlayersInMatch.Remove(Match->PlayerB);
    ActiveMatches.Remove(MatchID);
}

void UOlympiadSubsystem::TryMatchmaking()
{
    if (PlayerRatings.Num() < 2) return;
    if (ActiveMatches.Num() >= MAX_ACTIVE_MATCHES) return;

    TArray<FUniqueNetIdRepl> Players;
    PlayerRatings.GetKeys(Players);
    // Убираем тех, кто уже в матче
    Players.RemoveAll([this](const FUniqueNetIdRepl& P) { return PlayersInMatch.Contains(P); });

    // Сортируем по рейтингу
    Players.Sort([this](const FUniqueNetIdRepl& A, const FUniqueNetIdRepl& B)
    {
        return PlayerRatings[A].Rating < PlayerRatings[B].Rating;
    });

    for (int32 i = 0; i < Players.Num() - 1; i += 2)
    {
        if (ActiveMatches.Num() >= MAX_ACTIVE_MATCHES) break;
        CreateMatch(Players[i], Players[i + 1]);
    }
}

TArray<FOlympiadPlayerData> UOlympiadSubsystem::GetTopPlayers(int32 Count) const
{
    TArray<FOlympiadPlayerData> Result;
    TArray<FUniqueNetIdRepl> Players;
    PlayerRatings.GetKeys(Players);
    Players.Sort([this](const FUniqueNetIdRepl& A, const FUniqueNetIdRepl& B)
    {
        return PlayerRatings[A].Rating > PlayerRatings[B].Rating;
    });

    for (int32 i = 0; i < FMath::Min(Count, Players.Num()); ++i)
    {
        Result.Add(PlayerRatings[Players[i]]);
    }
    return Result;
}

void UOlympiadSubsystem::GiveWeeklyRewards()
{
    TArray<FOlympiadPlayerData> Top = GetTopPlayers(10);
    for (const FOlympiadPlayerData& Data : Top)
    {
        // Здесь нужно найти игрока и выдать титул, бафф и т.д.
        UE_LOG(LogTemp, Log, TEXT("Player %s gets Olympiad reward (rating %d)"), *Data.PlayerID.ToString(), Data.Rating);
    }
}

void UOlympiadSubsystem::RecordChampionsInChronicle()
{
    if (Chronicle)
    {
        TArray<FOlympiadPlayerData> Top = GetTopPlayers(10);
        // Сохранить в историю хроник
        UE_LOG(LogTemp, Log, TEXT("Olympiad champions recorded in chronicle."));
    }
}

void UOlympiadSubsystem::SaveState()
{
    if (!IsServer()) return;

    UOlympiadSaveGame* Save = Cast<UOlympiadSaveGame>(UGameplayStatics::CreateSaveGameObject(UOlympiadSaveGame::StaticClass()));
    if (!Save) return;

    Save->PlayerRatings = PlayerRatings;
    Save->NextMatchID = NextMatchID;
    Save->LastWeeklyReset = FDateTime::UtcNow();

    UGameplayStatics::SaveGameToSlot(Save, TEXT("Olympiad"), 0);
}

void UOlympiadSubsystem::LoadState()
{
    if (!IsServer()) return;

    if (!UGameplayStatics::DoesSaveGameExist(TEXT("Olympiad"), 0)) return;

    UOlympiadSaveGame* Load = Cast<UOlympiadSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Olympiad"), 0));
    if (!Load) return;

    PlayerRatings = Load->PlayerRatings;
    NextMatchID = Load->NextMatchID;

    // Восстановление состояния сезона? Это требует дополнительной логики.
    // Пока просто начнём новый сезон.
    StartOlympiadWeek();
}

void UOlympiadSubsystem::AutoSave()
{
    SaveState();
}
