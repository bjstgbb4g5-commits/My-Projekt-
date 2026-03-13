#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MountData.h"
#include "MountActor.generated.h"

UCLASS()
class MOUNTSYSTEMCORE_API AMountActor : public APawn
{
    GENERATED_BODY()

public:
    AMountActor();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category = "Mount")
    void SetMountData(const FMountData& InMountData);

    UFUNCTION(BlueprintCallable, Category = "Mount")
    void TakeDamage(float Damage, AController* Instigator = nullptr, bool bIsPvE = false);

    UFUNCTION(BlueprintCallable, Category = "Mount")
    void Heal(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Mount")
    void KillMount();

    UFUNCTION(BlueprintCallable, Category = "Mount")
    void RegenHealthInStable(float DeltaTime);

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Mount")
    FMountData MountData;

private:
    struct FRecentDamageEntry
    {
        float Time;
        float Damage;
    };
    TArray<FRecentDamageEntry> RecentDamage;

    void UpdateRecentDamage(float DeltaTime);
    void CheckPanic();
    void ApplyGenesAndMutations();
};
