#include "LiveEventSubsystem.h"
#include "LiveEventWorldState.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "WorldGeneratorCore/Public/WorldGeneratorSubsystem.h"
#include "NavigationSystem.h"
#include "AssetRegistry/AssetRegistryModule.h"

ULiveEventSubsystem::ULiveEventSubsystem()
{
}

void ULiveEventSubsystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // Подсистема не реплицируется, репликацию обеспечивает WorldStateActor
}

void ULiveEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (!IsServer()) return;

    WorldGen = GetWorld()->GetSubsystem<UWorldGeneratorSubsystem>();

    // Создаём реплицируемый актор
    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = TEXT("LiveEventWorldState");
    WorldStateActor = GetWorld()->SpawnActor<ALiveEventWorldState>(ALiveEventWorldState::StaticClass(), SpawnParams);
    if (!WorldStateActor)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn LiveEventWorldState!"));
    }

    LoadEventTemplatesFromAssets();
}

void ULiveEventSubsystem::Deinitialize()
{
    if (WorldStateActor && IsValid(WorldStateActor))
    {
        WorldStateActor->Destroy();
        WorldStateActor = nullptr;
    }
    EventTemplates.Empty();
    LastEventTimes.Empty();
    Super::Deinitialize();
}

bool ULiveEventSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool ULiveEventSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId ULiveEventSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(ULiveEventSubsystem, STATGROUP_Tickables);
}

void ULiveEventSubsystem::Tick(float DeltaTime)
{
    EventCheckAccumulator += DeltaTime;
    if (EventCheckAccumulator < EVENT_CHECK_INTERVAL)
        return;

    EventCheckAccumulator = 0.f;

    FDateTime Now = FDateTime::UtcNow();

    // Проверяем завершившиеся события
    TArray<int32> ToRemove;
    if (WorldStateActor)
    {
        for (auto& Pair : WorldStateActor->ActiveEvents)
        {
            if (Pair.Value.State == ELiveEventState::Active && Now >= Pair.Value.EndTime)
            {
                ToRemove.Add(Pair.Key);
            }
        }
    }

    for (int32 ID : ToRemove)
    {
        EndEvent(ID);
    }

    // Генерируем новые события
    if (WorldStateActor && WorldStateActor->ActiveEvents.Num() < MAX_ACTIVE_EVENTS)
    {
        GenerateWorldEvents();
    }
}

void ULiveEventSubsystem::LoadEventTemplatesFromAssets()
{
    // Загружаем все ассеты типа ULiveEventTemplate из папки /Game/Events
    IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();

    FARFilter Filter;
    Filter.ClassPaths.Add(FTopLevelAssetPath(TEXT("/Script/WorldEventCore"), TEXT("LiveEventTemplate")));
    Filter.bRecursiveClasses = true;
    Filter.PackagePaths.Add(FName("/Game/Events"));

    TArray<FAssetData> AssetList;
    AssetRegistry.GetAssets(Filter, AssetList);

    for (const FAssetData& Asset : AssetList)
    {
        ULiveEventTemplate* Template = Cast<ULiveEventTemplate>(Asset.GetAsset());
        if (Template)
        {
            EventTemplates.Add(Template);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Loaded %d event templates."), EventTemplates.Num());
}

int32 ULiveEventSubsystem::StartEvent(ULiveEventTemplate* Template, const FVector& Location)
{
    if (!IsServer() || !Template || !WorldStateActor) return -1;

    // Проверка кулдауна
    FDateTime* LastTime = LastEventTimes.Find(Template->EventName);
    if (LastTime)
    {
        if (FDateTime::UtcNow() < (*LastTime + FTimespan::FromMinutes(Template->CooldownMinutes)))
        {
            return -1;
        }
    }

    // Проверка допустимой зоны
    ELiveEventZone Zone = DetermineZone(Location);
    if (!Template->AllowedZones.Contains(Zone))
    {
        return -1;
    }

    // Проверка максимального количества одновременных событий этого типа
    int32 ActiveCount = 0;
    for (const auto& Pair : WorldStateActor->ActiveEvents)
    {
        if (Pair.Value.TemplateName == Template->EventName)
            ActiveCount++;
    }
    if (ActiveCount >= Template->MaxSimultaneous)
        return -1;

    FLiveEventInstance Event;
    Event.EventID = NextEventID++;
    Event.TemplateName = Template->EventName;
    Event.Location = Location;
    Event.State = ELiveEventState::Active;
    Event.StartTime = FDateTime::UtcNow();
    Event.EndTime = Event.StartTime + FTimespan::FromMinutes(Template->DurationMinutes);

    WorldStateActor->ActiveEvents.Add(Event.EventID, Event);

    LastEventTimes.Add(Template->EventName, FDateTime::UtcNow());

    ApplyEventStartEffects(Event);
    AnnounceEvent(FString::Printf(TEXT("Event started: %s"), *Template->EventName));

    return Event.EventID;
}

void ULiveEventSubsystem::EndEvent(int32 EventID)
{
    if (!IsServer() || !WorldStateActor) return;

    FLiveEventInstance* Event = WorldStateActor->ActiveEvents.Find(EventID);
    if (!Event) return;

    Event->State = ELiveEventState::Completed;
    ApplyEventEndEffects(*Event);
    AnnounceEvent(FString::Printf(TEXT("Event ended: %s"), *Event->TemplateName));

    WorldStateActor->ActiveEvents.Remove(EventID);
}

void ULiveEventSubsystem::ForceGenerateEvent()
{
    if (IsServer())
    {
        GenerateWorldEvents();
    }
}

void ULiveEventSubsystem::GenerateWorldEvents()
{
    if (!WorldStateActor || EventTemplates.Num() == 0) return;
    if (FMath::RandRange(0, 100) > 15) return; // 15% chance per tick

    int32 Index = FMath::RandRange(0, EventTemplates.Num() - 1);
    ULiveEventTemplate* Template = EventTemplates[Index];
    if (!Template) return;

    // Выбираем случайную допустимую зону
    if (Template->AllowedZones.Num() == 0) return;
    ELiveEventZone Zone = Template->AllowedZones[FMath::RandRange(0, Template->AllowedZones.Num() - 1)];
    FVector Location = GetRandomLocationInZone(Zone);

    StartEvent(Template, Location);
}

ELiveEventZone ULiveEventSubsystem::DetermineZone(const FVector& Location) const
{
    if (!WorldGen) return ELiveEventZone::Green;

    float Dist = Location.Size2D();
    if (Dist > 40000) return ELiveEventZone::Black;
    if (Dist > 30000) return ELiveEventZone::Red;
    if (Dist > 20000) return ELiveEventZone::Blue;
    return ELiveEventZone::Green;
}

FVector ULiveEventSubsystem::GetRandomLocationInZone(ELiveEventZone Zone) const
{
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSys) return FVector::ZeroVector;

    float CenterDist = 0.f;
    float Radius = 20000.f;
    switch (Zone)
    {
    case ELiveEventZone::Green: CenterDist = 10000; Radius = 15000; break;
    case ELiveEventZone::Blue:  CenterDist = 25000; Radius = 15000; break;
    case ELiveEventZone::Red:   CenterDist = 40000; Radius = 15000; break;
    case ELiveEventZone::Black: CenterDist = 55000; Radius = 15000; break;
    }

    float Angle = FMath::RandRange(0, 2 * PI);
    FVector2D Dir(FMath::Cos(Angle), FMath::Sin(Angle));
    FVector2D Candidate = Dir * CenterDist;
    FVector SearchCenter(Candidate.X, Candidate.Y, 0);

    FNavLocation NavLoc;
    bool bFound = NavSys->GetRandomPointInNavigableRadius(SearchCenter, Radius, NavLoc);
    return bFound ? NavLoc.Location : SearchCenter;
}

void ULiveEventSubsystem::ApplyEventStartEffects(const FLiveEventInstance& Event)
{
    UE_LOG(LogTemp, Log, TEXT("Applying start effects for event %s at %s"), *Event.TemplateName, *Event.Location.ToString());
    // Здесь может быть спавн мобов, боссов и т.д.
}

void ULiveEventSubsystem::ApplyEventEndEffects(const FLiveEventInstance& Event)
{
    UE_LOG(LogTemp, Log, TEXT("Applying end effects for event %s"), *Event.TemplateName);
    // Раздача наград, деспавн сущностей
}

void ULiveEventSubsystem::AnnounceEvent(const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("[Global Event] %s"), *Message);
    // Можно отправить клиентам через RPC или GameState
}
