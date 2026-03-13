#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponMasteryTypes.h"
#include "SkillEvolutionTypes.h"
#include "ElementAffinityTypes.h"
#include "CrystalTypes.h"
#include "GuildCareerTypes.h"
#include "ProfessionTypes.h"
#include "ReputationTypes.h"
#include "PlayerProgressionComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CHARACTERPROGRESSIONCORE_API UPlayerProgressionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPlayerProgressionComponent();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // ---------- Weapon Mastery ----------
    UPROPERTY(Replicated, BlueprintReadOnly)
    TMap<EWeaponType, FWeaponMasteryData> WeaponMastery;

    UFUNCTION(BlueprintCallable, Category = "Progression|Weapon")
    void AddWeaponXP(EWeaponType Weapon, float XP);

    UFUNCTION(BlueprintCallable, Category = "Progression|Weapon")
    int32 GetWeaponMasteryLevel(EWeaponType Weapon) const;

    UFUNCTION(BlueprintCallable, Category = "Progression|Weapon")
    TArray<int32> GetUnlockedSkills(EWeaponType Weapon) const;

    // ---------- Skill Evolution ----------
    UPROPERTY(Replicated, BlueprintReadOnly)
    TMap<int32, FSkillEvolution> SkillEvolutions;

    UFUNCTION(BlueprintCallable, Category = "Progression|Skill")
    void UnlockSkill(int32 SkillID, EWeaponType Weapon);

    UFUNCTION(BlueprintCallable, Category = "Progression|Skill")
    void EvolveSkill(int32 SkillID, int32 NewLevel, int32 ChosenModifierID = 0);

    UFUNCTION(BlueprintCallable, Category = "Progression|Skill")
    float GetSkillDamageModifier(int32 SkillID) const;

    // ---------- Element Affinity ----------
    UPROPERTY(Replicated, BlueprintReadOnly)
    TMap<EElementType, FElementAffinity> ElementAffinities;

    UFUNCTION(BlueprintCallable, Category = "Progression|Element")
    void AddElementXP(EElementType Element, float XP);

    UFUNCTION(BlueprintCallable, Category = "Progression|Element")
    int32 GetElementAffinityLevel(EElementType Element) const;

    // ---------- Crystal ----------
    UPROPERTY(Replicated, BlueprintReadOnly)
    int32 EquippedCrystalID = -1;

    UFUNCTION(BlueprintCallable, Category = "Progression|Crystal")
    void EquipCrystal(int32 CrystalID);

    UFUNCTION(BlueprintCallable, Category = "Progression|Crystal")
    void UnequipCrystal();

    UFUNCTION(BlueprintCallable, Category = "Progression|Crystal")
    FElementCrystal GetEquippedCrystalData() const;

    // ---------- Guild Career ----------
    UPROPERTY(Replicated, BlueprintReadOnly)
    FGuildCareerData GuildCareer;

    UFUNCTION(BlueprintCallable, Category = "Progression|Guild")
    void AddGuildProgress(float Progress);

    UFUNCTION(BlueprintCallable, Category = "Progression|Guild")
    void ChangeGuild(EGuildType NewGuild);

    UFUNCTION(BlueprintCallable, Category = "Progression|Guild")
    EGuildRank GetGuildRank() const { return GuildCareer.Rank; }

    // ---------- Profession ----------
    UPROPERTY(Replicated, BlueprintReadOnly)
    TMap<int32, FProfessionData> Professions;

    UFUNCTION(BlueprintCallable, Category = "Progression|Profession")
    void AddProfessionXP(int32 ProfessionID, float XP);

    UFUNCTION(BlueprintCallable, Category = "Progression|Profession")
    int32 GetProfessionLevel(int32 ProfessionID) const;

    // ---------- Reputation ----------
    UPROPERTY(Replicated, BlueprintReadOnly)
    TMap<int32, FReputationData> Reputations;

    UFUNCTION(BlueprintCallable, Category = "Progression|Reputation")
    void AddReputationXP(int32 FactionID, float XP);

    UFUNCTION(BlueprintCallable, Category = "Progression|Reputation")
    int32 GetReputationLevel(int32 FactionID) const;

    // ---------- Build (active skills) ----------
    UPROPERTY(Replicated, BlueprintReadOnly)
    TArray<int32> ActiveSkillIDs; // до 6 навыков

    UFUNCTION(BlueprintCallable, Category = "Progression|Build")
    void SetActiveSkills(const TArray<int32>& SkillIDs);

    // Вспомогательные методы
    bool TrySpendResources(const TMap<EResourceType, int32>& Cost) const; // может понадобиться

private:
    bool IsServer() const;
    void LevelUpWeapon(EWeaponType Weapon);
    void LevelUpElement(EElementType Element);
    void LevelUpGuild();
    void LevelUpProfession(int32 ProfessionID);
    void LevelUpReputation(int32 FactionID);

    // Константы XP (можно вынести в конфиг)
    static constexpr float WEAPON_XP_PER_KILL = 10.f;
    static constexpr float WEAPON_XP_COOLDOWN = 0.5f;
    static constexpr float ELEMENT_XP_PER_ACTION = 5.f;
    static constexpr float GUILD_XP_PER_QUEST = 20.f;
    static constexpr float REPUTATION_XP_PER_QUEST = 15.f;

    // Anti‑exploit
    TMap<EWeaponType, float> LastXPTime;
};
