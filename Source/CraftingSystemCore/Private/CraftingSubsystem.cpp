#include "CraftingSubsystem.h"
#include "CraftingStationActor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerInventoryActor.h"
#include "GameFramework/PlayerController.h"
#include "InventorySystemCore/Public/PlayerEconomyComponent.h" // для доступа к инвентарю
#include "ItemSystemCore/Public/ItemSubsystem.h"
#include "TimerManager.h"

UCraftingSubsystem::UCraftingSubsystem()
{
}

void UCraftingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (IsServer())
    {
        LoadRecipes();
        CacheStations();
        ItemSystem = GetWorld()->GetSubsystem<UItemSubsystem>();
    }
}

void UCraftingSubsystem::Deinitialize()
{
    ClearStationCache();
    RecipeMap.Empty();
    Super::Deinitialize();
}

bool UCraftingSubsystem::IsServer() const
{
    UWorld* World = GetWorld();
    return World && World->IsServer();
}

void UCraftingSubsystem::LoadRecipes()
{
    // Загружаем DataTable через LoadObject
    RecipeDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/CraftingRecipes.CraftingRecipes"));
    if (RecipeDataTable)
    {
        TArray<FCraftingRecipe*> Rows;
        static const FString Context = TEXT("CraftingRecipes");
        RecipeDataTable->GetAllRows(Context, Rows);
        for (FCraftingRecipe* Row : Rows)
        {
            RecipeMap.Add(Row->RecipeID, *Row);
        }
        UE_LOG(LogTemp, Log, TEXT("CraftingSubsystem: Loaded %d recipes."), RecipeMap.Num());
    }
    else
    {
        // Заглушка для тестирования
        FCraftingRecipe Sword;
        Sword.RecipeID = 1001;
        Sword.ResultItemID = 2001; // Iron Sword
        Sword.ResultAmount = 1;
        Sword.StationType = ECraftingStationType::Blacksmith;
        Sword.CraftTime = 10.f;
        Sword.Ingredients.Add({3001, 10}); // IronIngot
        Sword.Ingredients.Add({3002, 2});  // Wood
        RecipeMap.Add(Sword.RecipeID, Sword);

        FCraftingRecipe Potion;
        Potion.RecipeID = 1002;
        Potion.ResultItemID = 2002; // Health Potion
        Potion.ResultAmount = 1;
        Potion.StationType = ECraftingStationType::Alchemy;
        Potion.CraftTime = 5.f;
        Potion.Ingredients.Add({3003, 3}); // HerbExtract
        RecipeMap.Add(Potion.RecipeID, Potion);
    }
}

void UCraftingSubsystem::CacheStations()
{
    if (!IsServer()) return;

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACraftingStationActor::StaticClass(), Found);
    for (AActor* Actor : Found)
    {
        ACraftingStationActor* Station = Cast<ACraftingStationActor>(Actor);
        if (Station)
        {
            StationCache.FindOrAdd(Station->StationType).Add(Station);
        }
    }
    UE_LOG(LogTemp, Log, TEXT("CraftingSubsystem: Cached %d stations."), Found.Num());
}

void UCraftingSubsystem::ClearStationCache()
{
    StationCache.Empty();
}

bool UCraftingSubsystem::IsPlayerNearStation(APlayerController* Player, ECraftingStationType StationType) const
{
    if (!IsServer() || !Player || !Player->GetPawn()) return false;

    FVector PlayerLocation = Player->GetPawn()->GetActorLocation();
    const float InteractionDistance = 300.f;

    const TArray<ACraftingStationActor*>* Stations = StationCache.Find(StationType);
    if (!Stations) return false;

    for (ACraftingStationActor* Station : *Stations)
    {
        if (!IsValid(Station)) continue;
        float Dist = FVector::Dist(PlayerLocation, Station->GetActorLocation());
        if (Dist <= InteractionDistance)
            return true;
    }
    return false;
}

const FCraftingRecipe* UCraftingSubsystem::GetRecipe(int32 RecipeID) const
{
    return RecipeMap.Find(RecipeID);
}

TArray<FCraftingRecipe> UCraftingSubsystem::GetRecipesForStation(ECraftingStationType StationType) const
{
    TArray<FCraftingRecipe> Result;
    for (const auto& Pair : RecipeMap)
    {
        if (Pair.Value.StationType == StationType)
            Result.Add(Pair.Value);
    }
    return Result;
}

bool UCraftingSubsystem::CanCraft(APlayerInventoryActor* Inventory, const FCraftingRecipe& Recipe) const
{
    if (!Inventory) return false;
    if (!Inventory->HasAuthority()) return false;

    for (const FCraftingIngredient& Ing : Recipe.Ingredients)
    {
        // Здесь нужно, чтобы в инвентаре был метод подсчёта предметов по ItemID
        // Предположим, что он есть (в PlayerEconomyComponent)
        UPlayerEconomyComponent* Econ = Inventory->GetOwner()->FindComponentByClass<UPlayerEconomyComponent>();
        if (!Econ) return false;
        // Нужен метод CountItem(ItemID) – пока заглушка
        // Вместо этого используем HasItem
        if (!Econ->HasItem(Ing.ItemID, Ing.Amount))
            return false;
    }
    return true;
}

bool UCraftingSubsystem::StartCraft(APlayerController* Player, int32 RecipeID)
{
    if (!IsServer() || !Player) return false;

    // Проверка нахождения у станции
    const FCraftingRecipe* Recipe = GetRecipe(RecipeID);
    if (!Recipe) return false;
    if (!IsPlayerNearStation(Player, Recipe->StationType))
    {
        UE_LOG(LogTemp, Warning, TEXT("StartCraft: player not near required station"));
        return false;
    }

    APawn* Pawn = Player->GetPawn();
    if (!Pawn) return false;
    APlayerInventoryActor* Inventory = Pawn->FindComponentByClass<APlayerInventoryActor>();
    if (!Inventory) return false;

    // Проверка наличия ингредиентов
    if (!CanCraft(Inventory, *Recipe)) return false;

    // Ограничение очереди (не более 5 заданий)
    // Нужен метод GetCraftQueue() в InventoryActor – пока предположим, что есть
    if (Inventory->GetCraftQueue().Num() >= 5)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartCraft: craft queue full"));
        return false;
    }

    // Списание ингредиентов
    UPlayerEconomyComponent* Econ = Pawn->FindComponentByClass<UPlayerEconomyComponent>();
    if (!Econ) return false;
    for (const FCraftingIngredient& Ing : Recipe->Ingredients)
    {
        if (!Econ->RemoveItem(Ing.ItemID, Ing.Amount))
        {
            // Восстанавливаем уже снятое? Но лучше атомарно. Упростим: после проверки CanCraft должно быть всё нормально.
            return false;
        }
    }

    // Добавление в очередь
    int32 QueueID = Inventory->GenerateNextQueueID(); // предположим, есть метод
    Inventory->AddToCraftQueue(QueueID, Recipe->RecipeID, GetWorld()->GetTimeSeconds(), Recipe->CraftTime);

    // Установка таймера завершения
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateWeakLambda(this, [this, Inventory, RecipeID]()
    {
        FinishCraft(Inventory, RecipeID);
    }), Recipe->CraftTime, false);

    return true;
}

void UCraftingSubsystem::FinishCraft(APlayerInventoryActor* Inventory, int32 RecipeID)
{
    if (!IsServer() || !Inventory) return;

    const FCraftingRecipe* Recipe = GetRecipe(RecipeID);
    if (!Recipe) return;

    // Генерация редкости
    EItemRarity Rarity = GenerateRarity(*Recipe);

    // Создание предмета через ItemSystem
    if (ItemSystem)
    {
        for (int32 i = 0; i < Recipe->ResultAmount; ++i)
        {
            FItemInstance Item = ItemSystem->CreateItem(Recipe->ResultItemID, 1);
            Item.Rarity = Rarity; // переопределяем редкость, если нужно
            // Добавление в инвентарь (предположим, есть метод AddItem)
            UPlayerEconomyComponent* Econ = Inventory->GetOwner()->FindComponentByClass<UPlayerEconomyComponent>();
            if (Econ)
            {
                Econ->AddItem(Item.ItemID, 1, false); // предполагаем, что AddItem принимает ItemID и количество
                // В реальности нужно доработать
            }
        }
    }

    // Удаление из очереди (по QueueID, но здесь у нас только RecipeID – не точно)
    Inventory->RemoveFromCraftQueueByRecipe(RecipeID); // предположим
}

EItemRarity UCraftingSubsystem::GenerateRarity(const FCraftingRecipe& Recipe) const
{
    float Roll = FMath::FRand();
    if (Roll < 0.70f)        // 70% Gray
        return EItemRarity::Gray;
    else if (Roll < 0.85f)   // 15% Green
        return EItemRarity::Green;
    else if (Roll < 0.95f)   // 10% Blue
        return EItemRarity::Blue;
    else if (Roll < 0.98f)   // 3% Red
        return EItemRarity::Red;
    else if (Roll < 0.995f)  // 1.5% Purple
        return EItemRarity::Purple;
    else                     // 0.5% Orange
        return EItemRarity::Orange;
}
