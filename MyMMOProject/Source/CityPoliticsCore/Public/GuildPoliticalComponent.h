#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/DateTime.h"
#include "GuildPoliticalComponent.generated.h"

USTRUCT(BlueprintType)
struct FCouncilSeat
{
    GENERATED_BODY()

    UPROPERTY()
    class APlayerState* Member;

    UPROPERTY()
    FDateTime JoinedAt;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CITYPOLITICSCORE_API UGuildPoliticalComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGuildPoliticalComponent();

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Political")
    int32 GuildBudget;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Political")
    TArray<int32> ControlledDistricts;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Political")
    TArray<FCouncilSeat> CouncilMembers;

    UFUNCTION(BlueprintCallable, Category = "Political")
    bool TryAddCouncilMember(class APlayerState* Player);

    UFUNCTION(BlueprintCallable, Category = "Political")
    bool IsCouncilFull() const { return CouncilMembers.Num() >= 10; }

    UFUNCTION(BlueprintCallable, Category = "Political")
    void CleanupCouncil();

    UFUNCTION(BlueprintCallable, Category = "Political")
    void AddGuildBudget(int32 Amount);

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
