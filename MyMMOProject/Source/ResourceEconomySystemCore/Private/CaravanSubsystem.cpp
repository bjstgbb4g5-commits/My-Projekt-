#include "CaravanSubsystem.h"
#include "CaravanActor.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "ResourceEconomySystemCore/Public/ResourceNodeSubsystem.h"
#include "MarketplaceSystemCore/Public/MarketplaceSubsystem.h"
#include "PvPWarfareCore/Public/PvPBattleSubsystem.h"
#include "WorldTerritorySystemCore/Public/TerritoryManagerSubsystem.h"
#include "GuildSystemCore/Public/GuildSubsystem.h" // placeholder

UCaravanSubsystem::UCaravanSubsystem()
{
}

void UCaravanSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    // При необходимости загрузить сохранённые караваны (опционально)
}

void UCaravanSubsystem::Deinitialize()
{
    for (auto& Pair : ActiveCaravans)
    {
        if (Pair.Value.IsValid())
            Pair.Value->Destroy();
    }
    ActiveCaravans.Empty();
    Super::Deinitialize();
}

bool UCaravanSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UCaravanSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UCaravanSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UCaravanSubsystem, STATGROUP_Tickables);
}

void UCaravanSubsystem::Tick(float DeltaTime)
{
    // Очищаем невалидные указатели
    TArray<int32> ToRemove;
    for (auto& Pair : ActiveCaravans)
    {
        if (!Pair.Value.IsValid())
        {
            ToRemove.Add(Pair.Key);
        }
    }
    for (int32 ID : ToRemove)
    {
        ActiveCaravans.Remove(ID);
    }

    // Обновление караванов (прогресс обрабатывается в самом акторе)
}

int32 UCaravanSubsystem::CreateCaravan(FName GuildID, int32 OriginCity, int32 DestinationCity, const TArray<FCaravanCargo>& Cargo)
{
    if (!IsServer()) return -1;
    if (ActiveCaravans.Num() >= MAX_ACTIVE_CARAVANS)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot create caravan: max active caravans reached"));
        return -1;
    }

    // Проверить, что ресурсы есть в городе отправления (интеграция с экономикой) – пока пропускаем

    FVector SpawnLocation = FVector(0, 0, 0); // нужно будет вычислить по городу
    FActorSpawnParameters SpawnParams;
    ACaravanActor* Caravan = GetWorld()->SpawnActor<ACaravanActor>(ACaravanActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    if (!Caravan) return -1;

    int32 CaravanID = NextCaravanID++;
    FCaravanData Data;
    Data.OwnerGuild = GuildID;
    Data.OriginCity = OriginCity;
    Data.DestinationCity = DestinationCity;
    Data.Cargo = Cargo;
    Data.State = ECaravanState::Traveling;
    Data.Progress = 0.0f;
    Data.CurrentLocation = SpawnLocation;

    Caravan->InitializeCaravan(CaravanID, Data);
    ActiveCaravans.Add(CaravanID, Caravan);

    UE_LOG(LogTemp, Log, TEXT("Caravan %d created by guild %s"), CaravanID, *GuildID.ToString());

    return CaravanID;
}

bool UCaravanSubsystem::AttackCaravan(int32 CaravanID)
{
    if (!IsServer()) return false;

    TWeakObjectPtr<ACaravanActor>* CaravanPtr = ActiveCaravans.Find(CaravanID);
    if (!CaravanPtr || !CaravanPtr->IsValid()) return false;

    ACaravanActor* Caravan = CaravanPtr->Get();
    Caravan->SetUnderAttack(true);
    return true;
}

bool UCaravanSubsystem::DestroyCaravan(int32 CaravanID)
{
    if (!IsServer()) return false;

    TWeakObjectPtr<ACaravanActor>* CaravanPtr = ActiveCaravans.Find(CaravanID);
    if (!CaravanPtr || !CaravanPtr->IsValid()) return false;

    ACaravanActor* Caravan = CaravanPtr->Get();
    Caravan->CaravanData.State = ECaravanState::Destroyed;

    // Спавним лут
    SpawnLoot(Caravan->CaravanData, Caravan->GetActorLocation());

    Caravan->Destroy();
    ActiveCaravans.Remove(CaravanID);

    UE_LOG(LogTemp, Log, TEXT("Caravan %d destroyed, loot spawned."), CaravanID);
    return true;
}

bool UCaravanSubsystem::DeliverCaravan(int32 CaravanID)
{
    if (!IsServer()) return false;

    TWeakObjectPtr<ACaravanActor>* CaravanPtr = ActiveCaravans.Find(CaravanID);
    if (!CaravanPtr || !CaravanPtr->IsValid()) return false;

    ACaravanActor* Caravan = CaravanPtr->Get();
    Caravan->CaravanData.State = ECaravanState::Completed;

    // Передать ресурсы городу назначения
    TransferResourcesToDestination(Caravan->CaravanData);

    Caravan->Destroy();
    ActiveCaravans.Remove(CaravanID);

    UE_LOG(LogTemp, Log, TEXT("Caravan %d completed, resources delivered."), CaravanID);
    return true;
}

const FCaravanData* UCaravanSubsystem::GetCaravanData(int32 CaravanID) const
{
    const TWeakObjectPtr<ACaravanActor>* CaravanPtr = ActiveCaravans.Find(CaravanID);
    if (CaravanPtr && CaravanPtr->IsValid())
    {
        return &(*CaravanPtr)->CaravanData;
    }
    return nullptr;
}

TArray<int32> UCaravanSubsystem::GetActiveCaravanIDs() const
{
    TArray<int32> Keys;
    ActiveCaravans.GetKeys(Keys);
    return Keys;
}

void UCaravanSubsystem::SpawnLoot(const FCaravanData& Caravan, const FVector& Location)
{
    // Создаём несколько предметов лута, которые игроки могут подобрать
    UResourceNodeSubsystem* Resources = GetWorld()->GetSubsystem<UResourceNodeSubsystem>();
    if (Resources)
    {
        for (const FCaravanCargo& Cargo : Caravan.Cargo)
        {
            // Здесь можно создать временные узлы ресурсов или предметы
            UE_LOG(LogTemp, Log, TEXT("Loot: %d of resource %d"), Cargo.Amount, (int32)Cargo.ResourceType);
        }
    }
}

void UCaravanSubsystem::TransferResourcesToDestination(const FCaravanData& Caravan)
{
    // Передаём ресурсы в экономику города назначения
    // Интеграция с Marketplace или ResourceEconomy
    UE_LOG(LogTemp, Log, TEXT("Transferring resources to city %d"), Caravan.DestinationCity);
}
