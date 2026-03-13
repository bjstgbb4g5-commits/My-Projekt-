#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "LiveEventTypes.h"
#include "LiveEventTemplate.h"
#include "LiveEventSubsystem.generated.h"

class UWorldGeneratorSubsystem;
class ALiveEventWorldState;

UCLASS()
class WORLDEVENTCORE_API ULiveEventSubsystem
    : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    ULiveEventSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

    // Запустить событие по шаблону в указанной локации
    UFUNCTION(BlueprintCallable, Category = "LiveEvent")
    int32 StartEvent(ULiveEventTemplate* Template, const FVector& Location);

    // Завершить событие по ID
    UFUNCTION(BlueprintCallable, Category = "LiveEvent")
    void EndEvent(int32 EventID);

    // Принудительно сгенерировать случайное событие (можно вызывать из консоли)
    UFUNCTION(Exec, Category = "LiveEvent")
    void ForceGenerateEvent();

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    bool IsServer() const;
    void LoadEventTemplatesFromAssets();
    void GenerateWorldEvents();
    void ApplyEventStartEffects(const FLiveEventInstance& Event);
    void ApplyEventEndEffects(const FLiveEventInstance& Event);
    void AnnounceEvent(const FString& Message);

    ELiveEventZone DetermineZone(const FVector& Location) const;
    FVector GetRandomLocationInZone(ELiveEventZone Zone) const;

    UPROPERTY()
    TArray<ULiveEventTemplate*> EventTemplates;

    UPROPERTY()
    ALiveEventWorldState* WorldStateActor;

    int32 NextEventID = 1;
    float EventCheckAccumulator = 0.f;
    static constexpr float EVENT_CHECK_INTERVAL = 60.f;
    static constexpr int32 MAX_ACTIVE_EVENTS = 10;

    TMap<FString, FDateTime> LastEventTimes; // кулдауны по имени шаблона
    UPROPERTY()
    UWorldGeneratorSubsystem* WorldGen;
};
