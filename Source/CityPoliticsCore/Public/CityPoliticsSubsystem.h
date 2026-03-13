#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CityPoliticsTypes.h"
#include "CityPoliticsSubsystem.generated.h"

UCLASS()
class CITYPOLITICSCORE_API UCityPoliticsSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "City Politics")
    void RegisterCityActor(int32 CityId, AActor* CityActor);

    UFUNCTION(BlueprintCallable, Category = "City Politics")
    AActor* GetCityActor(int32 CityId) const;

    // Отношения между городами
    UFUNCTION(BlueprintCallable, Category = "City Politics")
    void ModifyRelation(int32 CityA, int32 CityB, float Delta);

    UFUNCTION(BlueprintCallable, Category = "City Politics")
    bool CanDeclareWar(int32 CityA, int32 CityB) const;

    UFUNCTION(BlueprintCallable, Category = "City Politics")
    void DeclareWar(int32 CityA, int32 CityB);

    UPROPERTY(Replicated, BlueprintReadOnly)
    TArray<FCityRelation> CityRelations;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY()
    TMap<int32, TWeakObjectPtr<AActor>> CityRegistry;
};
