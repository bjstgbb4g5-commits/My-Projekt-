#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AITypes.h"
#include "AISquadComponent.generated.h"

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AICORESYSTEM_API UAISquadComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAISquadComponent();

    UFUNCTION(BlueprintCallable, Category = "Squad")
    void SetLeader(AActor* NewLeader);

    UFUNCTION(BlueprintCallable, Category = "Squad")
    AActor* GetLeader() const;

    UFUNCTION(BlueprintCallable, Category = "Squad")
    void RegisterMember(AActor* Member);

    UFUNCTION(BlueprintCallable, Category = "Squad")
    void UnregisterMember(AActor* Member);

    UFUNCTION(BlueprintCallable, Category = "Squad")
    TArray<AActor*> GetSquadMembers() const { return SquadMembers; }

    UFUNCTION(BlueprintCallable, Category = "Squad")
    bool IsLeader() const { return Leader == GetOwner(); }

    UFUNCTION(BlueprintCallable, Category = "Squad")
    void SetSquadRole(EAISquadRole Role) { SquadRole = Role; }

    UFUNCTION(BlueprintCallable, Category = "Squad")
    EAISquadRole GetSquadRole() const { return SquadRole; }

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated)
    AActor* Leader;

    UPROPERTY(Replicated)
    TArray<AActor*> SquadMembers;

    UPROPERTY(Replicated)
    EAISquadRole SquadRole = EAISquadRole::Damage;
};
