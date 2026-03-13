#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PartyTypes.h"
#include "PartySubsystem.generated.h"

USTRUCT()
struct FPartyData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 PartyId = -1;

    UPROPERTY()
    FUniqueNetIdRepl LeaderId;

    UPROPERTY()
    TArray<FPartyMemberData> Members;
};

UCLASS()
class PARTYSYSTEMCORE_API UPartySubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Основные методы
    UFUNCTION(BlueprintCallable, Category = "Party")
    int32 CreateParty(APlayerState* Leader);

    UFUNCTION(BlueprintCallable, Category = "Party")
    bool SendPartyInvite(APlayerState* Inviter, APlayerState* Invitee);

    UFUNCTION(BlueprintCallable, Category = "Party")
    bool AcceptPartyInvite(APlayerState* Player, int32 PartyId);

    UFUNCTION(BlueprintCallable, Category = "Party")
    bool DeclinePartyInvite(APlayerState* Player, int32 PartyId);

    UFUNCTION(BlueprintCallable, Category = "Party")
    bool LeaveParty(APlayerState* Player);

    UFUNCTION(BlueprintCallable, Category = "Party")
    bool KickFromParty(APlayerState* Leader, APlayerState* Target);

    UFUNCTION(BlueprintCallable, Category = "Party")
    bool TransferLeadership(APlayerState* CurrentLeader, APlayerState* NewLeader);

    UFUNCTION(BlueprintCallable, Category = "Party")
    TArray<FPartyMemberData> GetPartyMembers(int32 PartyId) const;

    UFUNCTION(BlueprintCallable, Category = "Party")
    int32 GetPlayerPartyId(APlayerState* Player) const;

private:
    bool IsServer() const;
    bool IsPlayerInParty(const FUniqueNetIdRepl& PlayerId) const;

    UPROPERTY()
    TMap<int32, FPartyData> Parties;

    UPROPERTY()
    TMap<FUniqueNetIdRepl, int32> PlayerPartyMap;

    TMap<FUniqueNetIdRepl, TArray<FPartyInviteData>> PendingInvites; // не реплицируется, только сервер

    int32 NextPartyId = 1;
    static constexpr int32 MaxPartySize = 5;
    static constexpr float InviteTimeoutSeconds = 30.0f;
};
