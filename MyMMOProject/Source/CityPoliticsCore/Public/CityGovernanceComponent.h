#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoliticalEnums.h"
#include "Misc/DateTime.h"
#include "CityGovernanceComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CITYPOLITICSCORE_API UCityGovernanceComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCityGovernanceComponent();

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Governance")
    class APlayerState* CurrentMayor;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Governance")
    FDateTime TermEndTime;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Governance")
    bool bElectionActive;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Governance")
    FDateTime ElectionEndTime;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Governance")
    ECityDoctrine ActiveDoctrine;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Governance")
    float CityTaxPercent;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Governance")
    float Legitimacy;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Governance")
    TMap<class APlayerState*, int32> CandidateVotes;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Governance")
    TSet<class APlayerState*> VotedPlayers;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Governance")
    TArray<class APlayerState*> EliteCouncil;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Governance")
    FDateTime LastPardonTime;

    UFUNCTION(BlueprintCallable, Category = "Governance")
    void StartElection();

    UFUNCTION(BlueprintCallable, Category = "Governance")
    void CastVote(class APlayerState* Voter, class APlayerState* Candidate);

    UFUNCTION(BlueprintCallable, Category = "Governance")
    void FinalizeElection();

    UFUNCTION(BlueprintCallable, Category = "Governance")
    void TransferPower(class APlayerState* NewMayor);

    UFUNCTION(BlueprintCallable, Category = "Governance")
    void SetDoctrine(ECityDoctrine NewDoctrine);

    UFUNCTION(BlueprintCallable, Category = "Governance")
    void SetTax(float NewPercent);

    UFUNCTION(BlueprintCallable, Category = "Governance")
    void CheckGovernanceTimers();

    UFUNCTION(BlueprintCallable, Category = "Governance")
    bool CanPardon(class APlayerState* Target) const;

    UFUNCTION(BlueprintCallable, Category = "Governance")
    void Pardon(class APlayerState* Target);

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    bool IsServer() const;
    FDateTime GetUtcNow() const;
    void UpdateLegitimacy(float Delta);
    void AddToEliteCouncil(APlayerState* Player);
    void RemoveFromEliteCouncil(APlayerState* Player);
};
