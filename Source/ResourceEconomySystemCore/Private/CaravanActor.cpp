#include "CaravanActor.h"
#include "CaravanSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "WorldTerritorySystemCore/Public/TerritoryManagerSubsystem.h"
#include "Engine/World.h"

ACaravanActor::ACaravanActor()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    bAlwaysRelevant = true;
    NetUpdateFrequency = 5.0f;
}

void ACaravanActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACaravanActor, CaravanData);
}

void ACaravanActor::BeginPlay()
{
    Super::BeginPlay();
    if (HasAuthority())
    {
        CaravanSubsystem = GetWorld()->GetSubsystem<UCaravanSubsystem>();
        TerritorySystem = GetWorld()->GetSubsystem<UWorldTerritorySystemCore>(); // предположим, что есть такой
    }
}

void ACaravanActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!HasAuthority()) return;

    if (CaravanData.State == ECaravanState::Traveling)
    {
        // Прогресс увеличивается со временем (скорость можно вынести в конфиг)
        CaravanData.Progress += DeltaTime * 0.01f; // 100 секунд на весь путь
        UpdateLocation(GetActorLocation()); // если позиция меняется, то обновляем
        if (CaravanData.Progress >= 1.0f)
        {
            CaravanData.State = ECaravanState::Completed;
            if (CaravanSubsystem)
            {
                CaravanSubsystem->DeliverCaravan(CaravanData.CaravanID);
            }
        }
    }
}

void ACaravanActor::InitializeCaravan(int32 CaravanID, const FCaravanData& Data)
{
    if (!HasAuthority()) return;
    CaravanData = Data;
    CaravanData.CaravanID = CaravanID;
    CaravanData.State = ECaravanState::Traveling;
    CaravanData.Progress = 0.0f;
    UpdateTerritoryAndRisk();
}

void ACaravanActor::SetUnderAttack(bool bAttacked)
{
    if (!HasAuthority()) return;
    CaravanData.State = bAttacked ? ECaravanState::UnderAttack : ECaravanState::Traveling;
    if (bAttacked)
    {
        OnCaravanAttacked.Broadcast(CaravanData.CaravanID);
        // Можно также запустить таймер на случай, если атака затянется
        GetWorldTimerManager().SetTimer(AttackTimerHandle, FTimerDelegate::CreateLambda([this]()
        {
            if (CaravanData.State == ECaravanState::UnderAttack)
            {
                // Если слишком долго под атакой – караван уничтожается
                if (CaravanSubsystem)
                {
                    CaravanSubsystem->DestroyCaravan(CaravanData.CaravanID);
                }
            }
        }), 60.0f, false); // 60 секунд на атаку
    }
    else
    {
        GetWorldTimerManager().ClearTimer(AttackTimerHandle);
    }
}

void ACaravanActor::UpdateLocation(const FVector& NewLocation)
{
    CaravanData.CurrentLocation = NewLocation;
    UpdateTerritoryAndRisk();
}

void ACaravanActor::UpdateTerritoryAndRisk()
{
    if (!TerritorySystem) return;

    // Определяем текущую территорию по координатам
    // Для этого нужен метод в TerritoryManagerSubsystem, например, GetTerritoryIDAtLocation
    // Пока заглушка
    CaravanData.CurrentTerritoryID = 1; // пример

    // Риск зависит от владельца территории и отношения с гильдией каравана
    // Здесь можно использовать данные из TerritorySystem
    CaravanData.RiskLevel = ECaravanRiskLevel::Medium; // заглушка
}

void ACaravanActor::OnRep_CaravanData()
{
    // Клиент может обновить UI
}
