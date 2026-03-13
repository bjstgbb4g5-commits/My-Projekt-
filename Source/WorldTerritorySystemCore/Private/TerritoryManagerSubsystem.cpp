#include "TerritoryManagerSubsystem.h"
#include "Engine/World.h"
#include "GuildSystemCore/Public/GuildSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "TerritorySaveGame.h"
#include "Math/UnrealMathUtility.h"

UTerritoryManagerSubsystem::UTerritoryManagerSubsystem()
{
}

void UTerritoryManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (!IsServer()) return;

    GuildSystem = GetWorld()->GetSubsystem<UGuildSubsystem>();
    LoadTerritories();

    // Если нет сохранённых данных, создаём тестовые территории
    if (Territories.Num() == 0)
    {
        FTerritoryData Territory;
        Territory.TerritoryID = 1;
        Territory.TerritoryName = "Ember Valley";
        Territory.TerritoryType = ETerritoryType::ResourceZone;
        Territory.OwnerGuild = FName("None");
        Territory.Resource.ResourceType = EResourceType::Crystal;
        Territory.Resource.MaxDeposit = 200000;
        Territory.Resource.CurrentDeposit = 200000;
        Territories.Add(Territory.TerritoryID, Territory);
    }
}

void UTerritoryManagerSubsystem::Deinitialize()
{
    SaveTerritories();
    Territories.Empty();
    Super::Deinitialize();
}

bool UTerritoryManagerSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

bool UTerritoryManagerSubsystem::IsTickable() const
{
    return IsServer();
}

TStatId UTerritoryManagerSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UTerritoryManagerSubsystem, STATGROUP_Tickables);
}

void UTerritoryManagerSubsystem::Tick(float DeltaTime)
{
    ResourceRegenTimer += DeltaTime;
    if (ResourceRegenTimer >= RESOURCE_REGEN_INTERVAL)
    {
        ResourceRegenTimer = 0.f;
        RegenerateResources(DeltaTime);
    }
}

void UTerritoryManagerSubsystem::RegenerateResources(float DeltaTime)
{
    for (auto& Pair : Territories)
    {
        FResourceDeposit& Deposit = Pair.Value.Resource;
        if (Deposit.CurrentDeposit < Deposit.MaxDeposit)
        {
            float RegenAmount = Deposit.MaxDeposit * Deposit.RegenerationRate * DeltaTime;
            Deposit.CurrentDeposit = FMath::Min(
                Deposit.MaxDeposit,
                Deposit.CurrentDeposit + static_cast<int32>(RegenAmount)
            );
        }
    }
}

bool UTerritoryManagerSubsystem::CaptureTerritory(int32 TerritoryID, const FName& GuildID)
{
    if (!IsServer()) return false;

    FTerritoryData* Territory = Territories.Find(TerritoryID);
    if (!Territory) return false;

    Territory->OwnerGuild = GuildID;
    Territory->bUnderWar = false;
    SaveTerritories();
    return true;
}

bool UTerritoryManagerSubsystem::SetTaxRate(int32 TerritoryID, float NewTax)
{
    if (!IsServer()) return false;

    FTerritoryData* Territory = Territories.Find(TerritoryID);
    if (!Territory) return false;

    Territory->TaxRate = FMath::Clamp(NewTax, 0.0f, 0.20f);
    SaveTerritories();
    return true;
}

FTerritoryData* UTerritoryManagerSubsystem::GetTerritory(int32 TerritoryID)
{
    return Territories.Find(TerritoryID);
}

TArray<FTerritoryData> UTerritoryManagerSubsystem::GetGuildTerritories(const FName& GuildID) const
{
    TArray<FTerritoryData> Result;
    for (const auto& Pair : Territories)
    {
        if (Pair.Value.OwnerGuild == GuildID)
            Result.Add(Pair.Value);
    }
    return Result;
}

TArray<FTerritoryData> UTerritoryManagerSubsystem::GetAllTerritories() const
{
    TArray<FTerritoryData> Result;
    Territories.GenerateValueArray(Result);
    return Result;
}

void UTerritoryManagerSubsystem::SaveTerritories()
{
    if (!IsServer()) return;

    UTerritorySaveGame* SaveGame = Cast<UTerritorySaveGame>(UGameplayStatics::CreateSaveGameObject(UTerritorySaveGame::StaticClass()));
    if (!SaveGame) return;

    Territories.GenerateValueArray(SaveGame->Territories);
    UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("Territories"), 0);
}

void UTerritoryManagerSubsystem::LoadTerritories()
{
    if (!IsServer()) return;

    if (!UGameplayStatics::DoesSaveGameExist(TEXT("Territories"), 0)) return;

    UTerritorySaveGame* LoadGame = Cast<UTerritorySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Territories"), 0));
    if (!LoadGame) return;

    Territories.Empty();
    for (const FTerritoryData& Data : LoadGame->Territories)
    {
        Territories.Add(Data.TerritoryID, Data);
    }
}
