#include "LiveEventSubsystem.h"
#include "LiveEventWorldState.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "WorldGeneratorCore/Public/WorldGeneratorSubsystem.h"
#include "NavigationSystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "TimerManager.h"

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

    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = TEXT("LiveEventWorldState");
    WorldStateActor = GetWorld()->SpawnActor<ALiveEventWorldState>(ALiveEventWorldState::StaticClass(), SpawnParams);
    if (!WorldStateActor)
    {
        UE_LOG(LogTemp, Error, TEXT("ULiveEventSubsystem: Failed to spawn LiveEventWorldState!"));
        return;
    }

    LoadEventTemplatesFromAssets();

    GetWorld()->GetTimerManager().SetTimer(CooldownCleanupTimer, this, &ULiveEventSubsystem::CleanupCooldowns, 3600.0f, true);
}

void ULiveEventSubsystem::Deinitialize()
{
    if (IsServer())
    {
        GetWorld()->GetTimerManager().ClearTimer(CooldownCleanupTimer);
    }

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
    return IsServer() && WorldStateActor != nullptr;
}

TStatId ULiveEventSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(ULiveEventSubsystem, STATGROUP_Tickables);
}

void ULiveEventSubsystem::Tick(float DeltaTime)
{
    if (!WorldStateActor) return;

    EventCheckAccumulator += DeltaTime;
    if (EventCheckAccumulator < EVENT_CHECK_INTERVAL)
        return;

    EventCheckAccumulator = 0.f;

    FDateTime Now = FDateTime::UtcNow();

    TArray<int32> ToRemove;
    for (const auto& Pair : WorldStateActor->ActiveEvents)
    {
        if (Pair.Value.State == ELiveEventState::Active && Now >= Pair.Value.EndTime)
        {
            ToRemove.Add(Pair.Key);
        }
    }

    for (int32 ID : ToRemove)
    {
        EndEvent(ID);
    }

    if (WorldStateActor->ActiveEvents.Num() < MAX_ACTIVE_EVENTS)
    {
        GenerateWorldEvents();
    }
}

void ULiveEventSubsystem::LoadEventTemplatesFromAssets()
{
    IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();

    FARFilter Filter;
    Filter.ClassPaths.Add(FTopLevelAssetPath(TEXT("/Script/LiveEventSystemCore"), TEXT("LiveEventTemplate")));
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

    UE_LOG(LogTemp, Log, TEXT("ULiveEventSubsystem: Loaded %d event templates."), EventTemplates.Num());
}

int32 ULiveEventSubsystem::StartEvent(ULiveEventTemplate* Template, const FVector& Location)
{
    if (!IsServer() || !Template || !WorldStateActor) return -1;

    // Проверка, не превышен ли лимит одновременных событий этого типа
    int32 ActiveCount = 0;
    for (const auto& Pair : WorldStateActor->ActiveEvents)
    {
        if (Pair.Value.TemplateName == Template->EventName)
            ActiveCount++;
    }
    if (ActiveCount >= Template->MaxSimultaneous)
        return -1;

    // Проверка кулдауна
    FDateTime* LastTime = LastEventTimes.Find(Template->EventName);
    if (LastTime && FDateTime::UtcNow() < (*LastTime + FTimespan::FromMinutes(Template->CooldownMinutes)))
        return -1;

    // Проверка допустимой зоны
    ELiveEventZone Zone = DetermineZone(Location);
    if (!Template->AllowedZones.Contains(Zone))
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
    AnnounceEvent(FString::Printf(TEXT("Event started: %s at %s"), *Template->EventName, *Location.ToString()));

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

void ULiveEventSubsystem::CleanupCooldowns()
{
    FDateTime Now = FDateTime::UtcNow();
    TArray<FString> ToRemove;
    for (const auto& Pair : LastEventTimes)
    {
        if ((Now - Pair.Value).GetTotalDays() > 7)
            ToRemove.Add(Pair.Key);
    }
    for (const FString& Key : ToRemove)
        LastEventTimes.Remove(Key);
}

TArray<FLiveEventInstance> ULiveEventSubsystem::GetActiveEvents() const
{
    TArray<FLiveEventInstance> Result;
    if (WorldStateActor)
    {
        WorldStateActor->ActiveEvents.GenerateValueArray(Result);
    }
    return Result;
}

void ULiveEventSubsystem::GenerateWorldEvents()
{
    if (!WorldStateActor || EventTemplates.Num() == 0) return;
    if (FMath::RandRange(0, 100) > 15) return;

    int32 Index = FMath::RandRange(0, EventTemplates.Num() - 1);
    ULiveEventTemplate* Template = EventTemplates[Index];
    if (!Template) return;

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
    SearchCenter.X += FMath::RandRange(-5000.f, 5000.f);
    SearchCenter.Y += FMath::RandRange(-5000.f, 5000.f);

    FNavLocation NavLoc;
    bool bFound = NavSys->GetRandomPointInNavigableRadius(SearchCenter, Radius, NavLoc);
    return bFound ? NavLoc.Location : SearchCenter;
}

void ULiveEventSubsystem::ApplyEventStartEffects(const FLiveEventInstance& Event)
{
    UE_LOG(LogTemp, Log, TEXT("Applying start effects for event %s"), *Event.TemplateName);
    // Здесь может быть спавн мобов, боссов и т.д.
}

void ULiveEventSubsystem::ApplyEventEndEffects(const FLiveEventInstance& Event)
{
    UE_LOG(LogTemp, Log, TEXT("Applying end effects for event %s"), *Event.TemplateName);
    // Раздача наград, деспавн сущностей
}

void ULiveEventSubsystem::AnnounceEvent(const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("[Live Event] %s"), *Message);
    // Можно отправить клиентам через RPC или GameState
}
