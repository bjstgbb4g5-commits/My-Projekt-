#include "ChronicleSubsystem.h"
#include "ChronicleSaveGame.h"
#include "ChronicleSeasonHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/DateTime.h"
#include "ApostleSubsystem.h" // для вызова спавна апостолов
#include "NoxEventSubsystem.h" // для финального события

UChronicleSubsystem::UChronicleSubsystem()
{
}

void UChronicleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (!IsServer()) return;

    if (!LoadChronicleState(TEXT("ChronicleSave")))
    {
        ChronicleData.Chronicle = EChronicleID::Chronicle1;
        ChronicleData.CurrentSeason = 1;
        ChronicleData.bNoxDefeated = false;
        ChronicleData.StartDate = GetUtcNow();
        ChronicleData.EndDate = ChronicleData.StartDate + FTimespan::FromDays(56 * 6); // 6 сезонов по 8 недель
        StartSeason(1);
    }
}

void UChronicleSubsystem::Deinitialize()
{
    Seasons.Empty();
    Super::Deinitialize();
}

bool UChronicleSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

FDateTime UChronicleSubsystem::GetUtcNow() const
{
    return FDateTime::UtcNow();
}

bool UChronicleSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UChronicleSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UChronicleSubsystem, STATGROUP_Tickables);
}

void UChronicleSubsystem::Tick(float DeltaTime)
{
    SeasonCheckAccumulator += DeltaTime;
    if (SeasonCheckAccumulator < SEASON_CHECK_INTERVAL)
        return;
    SeasonCheckAccumulator = 0.f;

    if (Seasons.Num() > 0)
    {
        FSeasonData& CurrentSeason = Seasons.Last();
        if (CurrentSeason.State == ESeasonState::Active && GetUtcNow() >= CurrentSeason.EndTime)
        {
            EndSeason();
        }
    }
}

FDateTime UChronicleSubsystem::GetSeasonEndTime() const
{
    if (Seasons.Num() > 0)
        return Seasons.Last().EndTime;
    return FDateTime::MinValue();
}

void UChronicleSubsystem::StartSeason(int32 SeasonIndex)
{
    if (!IsServer()) return;

    ESeasonType SeasonType = GetSeasonTypeByIndex(SeasonIndex);

    FSeasonData Season;
    Season.SeasonIndex = SeasonIndex;
    Season.SeasonType = SeasonType;
    Season.State = ESeasonState::Active;
    Season.StartTime = GetUtcNow();
    Season.EndTime = Season.StartTime + FTimespan::FromDays(56);

    Seasons.Add(Season);

    UE_LOG(LogTemp, Log, TEXT("Season %d started (type %d), ends at %s"), SeasonIndex, (int32)SeasonType, *Season.EndTime.ToString());

    OnSeasonStarted.Broadcast(SeasonIndex);
    OnSeasonStart();

    SaveChronicleState(TEXT("ChronicleSave"));
}

void UChronicleSubsystem::EndSeason()
{
    if (!IsServer() || Seasons.Num() == 0) return;

    FSeasonData& CurrentSeason = Seasons.Last();
    CurrentSeason.State = ESeasonState::Finished;

    UE_LOG(LogTemp, Log, TEXT("Season %d ended."), CurrentSeason.SeasonIndex);

    OnSeasonEnd();

    ChronicleData.CurrentSeason = CurrentSeason.SeasonIndex + 1;

    if (ChronicleData.CurrentSeason > 6)
    {
        OnChronicleEnd();
    }
    else
    {
        StartSeason(ChronicleData.CurrentSeason);
    }
}

void UChronicleSubsystem::StartNextChronicle()
{
    if (bChronicleTransition) return;
    bChronicleTransition = true;

    int32 NextChronicle = (int32)ChronicleData.Chronicle + 1;
    if (NextChronicle > (int32)EChronicleID::Chronicle6)
    {
        UE_LOG(LogTemp, Warning, TEXT("All chronicles completed."));
        bChronicleTransition = false;
        return;
    }

    ChronicleData.Chronicle = (EChronicleID)NextChronicle;
    ChronicleData.CurrentSeason = 1;
    ChronicleData.bNoxDefeated = false;

    OnChronicleStarted.Broadcast(ChronicleData.Chronicle);
    StartSeason(1);
    bChronicleTransition = false;
}

void UChronicleSubsystem::OnSeasonStart()
{
    UWorld* World = GetWorld();
    if (!World) return;

    if (Seasons.Num() == 0) return;
    const FSeasonData& Season = Seasons.Last();

    // Если сезон апостола (1-5) – спавним соответствующего апостола
    UApostleSubsystem* ApostleSystem = World->GetSubsystem<UApostleSubsystem>();
    if (ApostleSystem && Season.SeasonType >= ESeasonType::ApostleWar && Season.SeasonType <= ESeasonType::ApostleFlame)
    {
        // Определим координаты спавна (можно брать из конфига)
        FVector SpawnLocation = FVector(100000 * Season.SeasonIndex, 50000, 0); // пример
        ApostleSystem->SpawnApostle((EApostleType)Season.SeasonType, SpawnLocation);
    }

    // Если финальный сезон – запускаем событие Нокса
    if (Season.SeasonType == ESeasonType::NoxSeason)
    {
        UNoxEventSubsystem* NoxSystem = World->GetSubsystem<UNoxEventSubsystem>();
        if (NoxSystem)
        {
            NoxSystem->StartNoxEvent();
        }
    }
}

void UChronicleSubsystem::OnSeasonEnd()
{
    UWorld* World = GetWorld();
    if (!World) return;

    if (Seasons.Num() == 0) return;
    const FSeasonData& Season = Seasons.Last();

    // Если апостольский сезон – можно отметить, что апостол мёртв (подсистема сама отследит)
    // Ничего не делаем, ApostleSubsystem уже обновлён.

    // Если финальный сезон и Нокс побеждён – генерируем осколки и открываем новые зоны
    if (Season.SeasonType == ESeasonType::NoxSeason && ChronicleData.bNoxDefeated)
    {
        UNoxEventSubsystem* NoxSystem = World->GetSubsystem<UNoxEventSubsystem>();
        if (NoxSystem)
        {
            // Осколки уже сгенерированы в CompleteNoxEvent
        }
        // Открыть Neo Aurelion, Tenebris и т.д.
    }
}

void UChronicleSubsystem::OnChronicleEnd()
{
    StartNextChronicle();
}

void UChronicleSubsystem::SaveChronicleState(const FString& SlotName)
{
    if (!IsServer()) return;

    UChronicleSaveGame* SaveGame = Cast<UChronicleSaveGame>(UGameplayStatics::CreateSaveGameObject(UChronicleSaveGame::StaticClass()));
    if (!SaveGame) return;

    SaveGame->Chronicle = ChronicleData.Chronicle;
    SaveGame->CurrentSeason = ChronicleData.CurrentSeason;
    SaveGame->SeasonStartTime = Seasons.Num() > 0 ? Seasons.Last().StartTime : GetUtcNow();
    SaveGame->bNoxDefeated = ChronicleData.bNoxDefeated;

    UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
    UE_LOG(LogTemp, Log, TEXT("Chronicle saved to slot %s."), *SlotName);
}

bool UChronicleSubsystem::LoadChronicleState(const FString& SlotName)
{
    if (!IsServer()) return false;

    if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0)) return false;

    UChronicleSaveGame* LoadGame = Cast<UChronicleSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if (!LoadGame) return false;

    ChronicleData.Chronicle = LoadGame->Chronicle;
    ChronicleData.CurrentSeason = LoadGame->CurrentSeason;
    ChronicleData.bNoxDefeated = LoadGame->bNoxDefeated;

    FDateTime Now = GetUtcNow();
    FSeasonData Season;
    Season.SeasonIndex = LoadGame->CurrentSeason;
    Season.SeasonType = GetSeasonTypeByIndex(Season.SeasonIndex);
    Season.State = ESeasonState::Active;
    Season.StartTime = LoadGame->SeasonStartTime;
    Season.EndTime = Season.StartTime + FTimespan::FromDays(56);
    Seasons.Add(Season);

    if (Now >= Season.EndTime)
    {
        EndSeason();
    }

    return true;
}
