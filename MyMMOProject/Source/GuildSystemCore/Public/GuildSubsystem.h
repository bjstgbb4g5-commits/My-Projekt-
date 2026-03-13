#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GuildTypes.h"
#include "GuildSubsystem.generated.h"

UCLASS()
class GUILDSYSTEMCORE_API UGuildSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Создать гильдию
    UFUNCTION(BlueprintCallable, Category = "Guild")
    int32 CreateGuild(const FString& GuildName, const FString& Tag, APlayerState* Founder);

    // Получить данные гильдии
    UFUNCTION(BlueprintCallable, Category = "Guild")
    const FGuildData* GetGuild(int32 GuildId) const;

    // Получить гильдию игрока
    UFUNCTION(BlueprintCallable, Category = "Guild")
    int32 GetPlayerGuildId(APlayerState* Player) const;

    // Добавить игрока в гильдию (после принятия приглашения)
    UFUNCTION(BlueprintCallable, Category = "Guild")
    bool AddMember(int32 GuildId, APlayerState* Player, EGuildRank Rank);

    // Удалить игрока
    UFUNCTION(BlueprintCallable, Category = "Guild")
    bool RemoveMember(int32 GuildId, APlayerState* Player);

    // Повысить/понизить ранг
    UFUNCTION(BlueprintCallable, Category = "Guild")
    bool SetMemberRank(int32 GuildId, APlayerState* Player, EGuildRank NewRank);

    // Банковские операции
    UFUNCTION(BlueprintCallable, Category = "Guild")
    bool DepositItem(int32 GuildId, APlayerState* Player, int32 ItemID, int32 Quantity);

    UFUNCTION(BlueprintCallable, Category = "Guild")
    bool WithdrawItem(int32 GuildId, APlayerState* Player, int32 ItemID, int32 Quantity);

    // Сохранение и загрузка
    void SaveGuilds();
    void LoadGuilds();

private:
    bool IsServer() const;

    UPROPERTY()
    TMap<int32, FGuildData> Guilds;

    UPROPERTY()
    TMap<FUniqueNetIdRepl, int32> PlayerGuildMap; // быстрый поиск

    int32 NextGuildId = 1;
};
