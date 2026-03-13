#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CityPoliticalComponent.generated.h"

USTRUCT(BlueprintType)
struct FDistrictData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DistrictId = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BaseGuildOwner = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InfluenceGuildId = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float InfluenceValue = 0.f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CITYPOLITICSCORE_API UCityPoliticalComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCityPoliticalComponent();

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Political")
    float Prosperity;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Political")
    float Loyalty;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Political")
    float Unrest;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Political")
    TArray<FDistrictData> Districts;

    UFUNCTION(BlueprintCallable, Category = "Political")
    void ModifyProsperity(float Delta);

    UFUNCTION(BlueprintCallable, Category = "Political")
    void ModifyLoyalty(float Delta);

    UFUNCTION(BlueprintCallable, Category = "Political")
    void ModifyUnrest(float Delta);

    UFUNCTION(BlueprintCallable, Category = "Political")
    void ModifyInfluence(int32 DistrictId, int32 GuildId, float Delta);

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    void ClampValues();
};
