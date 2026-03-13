#include "CityGovernanceComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "PoliticalIdentityComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

UCityGovernanceComponent::UCityGovernanceComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);

    CurrentMayor = nullptr;
    bElectionActive = false;
    ActiveDoctrine = ECityDoctrine::None;
    CityTaxPercent = 15.f;
    Legitimacy = 50.f;
}

void UCityGovernanceComponent::BeginPlay()
{
    Super::BeginPlay();
    if (IsServer())
    {
        CheckGovernanceTimers();
    }
}

void UCityGovernanceComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UCityGovernanceComponent, CurrentMayor);
    DOREPLIFETIME(UCityGovernanceComponent, TermEndTime);
    DOREPLIFETIME(UCityGovernanceComponent, bElectionActive);
    DOREPLIFETIME(UCityGovernanceComponent, ElectionEndTime);
    DOREPLIFETIME(UCityGovernanceComponent, ActiveDoctrine);
    DOREPLIFETIME(UCityGovernanceComponent, CityTaxPercent);
    DOREPLIFETIME(UCityGovernanceComponent, Legitimacy);
    DOREPLIFETIME(UCityGovernanceComponent, CandidateVotes);
    DOREPLIFETIME(UCityGovernanceComponent, VotedPlayers);
    DOREPLIFETIME(UCityGovernanceComponent, EliteCouncil);
    DOREPLIFETIME(UCityGovernanceComponent, LastPardonTime);
}

bool UCityGovernanceComponent::IsServer() const
{
    return GetOwner() && GetOwner()->HasAuthority();
}

FDateTime UCityGovernanceComponent::GetUtcNow() const
{
    return FDateTime::UtcNow();
}

void UCityGovernanceComponent::StartElection()
{
    if (!IsServer()) return;
    if (bElectionActive) return;

    // Сбор кандидатов: главы гильдий (упрощённо добавляем несколько)
    TArray<APlayerState*> Candidates;
    if (CurrentMayor) Candidates.Add(CurrentMayor);
    for (int32 i = 0; i < FMath::Min(2, EliteCouncil.Num()); ++i)
    {
        Candidates.Add(EliteCouncil[i]);
    }

    // Проверка на ограничение по срокам
    Candidates.RemoveAll([this](APlayerState* Candidate) {
        if (!Candidate) return true;
        UPoliticalIdentityComponent* Id = Candidate->GetPawn() ? Candidate->GetPawn()->FindComponentByClass<UPoliticalIdentityComponent>() : nullptr;
        return Id && Id->ConsecutiveMayorTerms >= 2;
    });

    if (Candidates.Num() == 0) return;

    CandidateVotes.Empty();
    VotedPlayers.Empty();
    for (APlayerState* Cand : Candidates)
    {
        CandidateVotes.Add(Cand, 0);
    }

    bElectionActive = true;
    ElectionEndTime = GetUtcNow() + FTimespan::FromHours(24);
}

void UCityGovernanceComponent::CastVote(APlayerState* Voter, APlayerState* Candidate)
{
    if (!IsServer()) return;
    if (!bElectionActive) return;
    if (!Voter || !Candidate) return;

    UPoliticalIdentityComponent* VoterId = Voter->GetPawn() ? Voter->GetPawn()->FindComponentByClass<UPoliticalIdentityComponent>() : nullptr;
    if (!VoterId || VoterId->RegisteredCityId != GetOwner()->GetUniqueID()) return;

    if (VotedPlayers.Contains(Voter)) return;
    if (!CandidateVotes.Contains(Candidate)) return;

    CandidateVotes[Candidate] += 1;
    VotedPlayers.Add(Voter);
}

void UCityGovernanceComponent::FinalizeElection()
{
    if (!IsServer()) return;
    if (!bElectionActive) return;
    if (GetUtcNow() < ElectionEndTime) return;

    APlayerState* Winner = nullptr;
    int32 MaxVotes = -1;
    TArray<APlayerState*> Leaders;

    for (auto& Pair : CandidateVotes)
    {
        if (Pair.Value > MaxVotes)
        {
            MaxVotes = Pair.Value;
            Leaders.Empty();
            Leaders.Add(Pair.Key);
        }
        else if (Pair.Value == MaxVotes)
        {
            Leaders.Add(Pair.Key);
        }
    }

    if (Leaders.Num() > 0)
    {
        int32 RandIdx = FMath::RandRange(0, Leaders.Num() - 1);
        Winner = Leaders[RandIdx];
    }

    bElectionActive = false;
    CandidateVotes.Empty();
    VotedPlayers.Empty();

    if (Winner)
    {
        TransferPower(Winner);
    }
}

void UCityGovernanceComponent::TransferPower(APlayerState* NewMayor)
{
    if (!IsServer()) return;
    if (!NewMayor) return;

    if (CurrentMayor)
    {
        UPoliticalIdentityComponent* OldId = CurrentMayor->GetPawn() ? CurrentMayor->GetPawn()->FindComponentByClass<UPoliticalIdentityComponent>() : nullptr;
        if (OldId) OldId->PoliticalRank = EPoliticalRank::Elite;
        AddToEliteCouncil(CurrentMayor);
    }

    CurrentMayor = NewMayor;
    UPoliticalIdentityComponent* NewId = NewMayor->GetPawn() ? NewMayor->GetPawn()->FindComponentByClass<UPoliticalIdentityComponent>() : nullptr;
    if (NewId)
    {
        if (NewId->PoliticalRank == EPoliticalRank::Mayor)
            NewId->ConsecutiveMayorTerms++;
        else
            NewId->PoliticalRank = EPoliticalRank::Mayor;
        NewId->ConsecutiveMayorTerms = 1;
    }

    TermEndTime = GetUtcNow() + FTimespan::FromDays(14);
    Legitimacy = 50.f;

    RemoveFromEliteCouncil(NewMayor);
}

void UCityGovernanceComponent::SetDoctrine(ECityDoctrine NewDoctrine)
{
    if (!IsServer()) return;
    if (CurrentMayor == nullptr) return;
    ActiveDoctrine = NewDoctrine;
}

void UCityGovernanceComponent::SetTax(float NewPercent)
{
    if (!IsServer()) return;
    if (CurrentMayor == nullptr) return;
    NewPercent = FMath::Clamp(NewPercent, 10.f, 25.f);
    float Old = CityTaxPercent;
    CityTaxPercent = NewPercent;

    if (NewPercent > 20.f && Old <= 20.f)
    {
        UpdateLegitimacy(-5.f);
        // Увеличить Unrest
        if (UCityPoliticalComponent* Pol = GetOwner()->FindComponentByClass<UCityPoliticalComponent>())
            Pol->ModifyUnrest(5.f);
    }
}

void UCityGovernanceComponent::CheckGovernanceTimers()
{
    if (!IsServer()) return;
    FDateTime Now = GetUtcNow();

    if (CurrentMayor && Now >= TermEndTime)
    {
        if (UPoliticalIdentityComponent* Id = CurrentMayor->GetPawn() ? CurrentMayor->GetPawn()->FindComponentByClass<UPoliticalIdentityComponent>() : nullptr)
        {
            Id->PoliticalRank = EPoliticalRank::Elite;
        }
        AddToEliteCouncil(CurrentMayor);
        CurrentMayor = nullptr;
        TermEndTime = FDateTime();
    }

    if (bElectionActive && Now >= ElectionEndTime)
    {
        FinalizeElection();
    }
}

bool UCityGovernanceComponent::CanPardon(APlayerState* Target) const
{
    if (!IsServer()) return false;
    if (!Target || Target == CurrentMayor) return false;
    // Проверка на состояние ожидания казни и кулдаун
    FDateTime Now = GetUtcNow();
    if (LastPardonTime + FTimespan::FromHours(48) > Now) return false;
    // Здесь можно добавить проверку через JusticeComponent, что не в ожидании казни
    return true;
}

void UCityGovernanceComponent::Pardon(APlayerState* Target)
{
    if (!IsServer()) return;
    if (!CanPardon(Target)) return;

    // Сброс состояния JusticeComponent
    // (будет через UJusticeComponent, пока заглушка)
    UpdateLegitimacy(-5.f);
    if (UCityPoliticalComponent* Pol = GetOwner()->FindComponentByClass<UCityPoliticalComponent>())
        Pol->ModifyUnrest(5.f);

    LastPardonTime = GetUtcNow();
}

void UCityGovernanceComponent::UpdateLegitimacy(float Delta)
{
    Legitimacy = FMath::Clamp(Legitimacy + Delta, 0.f, 100.f);
}

void UCityGovernanceComponent::AddToEliteCouncil(APlayerState* Player)
{
    if (Player) EliteCouncil.AddUnique(Player);
}

void UCityGovernanceComponent::RemoveFromEliteCouncil(APlayerState* Player)
{
    EliteCouncil.Remove(Player);
}
