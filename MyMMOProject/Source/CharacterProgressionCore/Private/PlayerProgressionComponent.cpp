#include "PlayerProgressionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "WeaponMasterySubsystem.h"
#include "SkillEvolutionSubsystem.h"
#include "ElementAffinitySubsystem.h"
#include "CrystalSubsystem.h"
#include "GuildCareerSubsystem.h"
#include "ProfessionSubsystem.h"
#include "ReputationSubsystem.h"

UPlayerProgressionComponent::UPlayerProgressionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UPlayerProgressionComponent::BeginPlay()
{
    Super::BeginPlay();
    if (IsServer())
    {
        // Инициализация начальных данных (опционально)
        for (int32 i = 0; i < (int32)EWeaponType::MAX; ++i)
        {
            EWeaponType Weapon = (EWeaponType)i;
            if (!WeaponMastery.Contains(Weapon))
            {
                FWeaponMasteryData Data;
                Data.WeaponType = Weapon;
                Data.MasteryLevel = 1;
                Data.MasteryXP = 0;
                WeaponMastery.Add(Weapon, Data);
            }
        }
    }
}

void UPlayerProgressionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UPlayerProgressionComponent, WeaponMastery, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UPlayerProgressionComponent, SkillEvolutions, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UPlayerProgressionComponent, ElementAffinities, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UPlayerProgressionComponent, EquippedCrystalID, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UPlayerProgressionComponent, GuildCareer, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UPlayerProgressionComponent, Professions, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UPlayerProgressionComponent, Reputations, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UPlayerProgressionComponent, ActiveSkillIDs, COND_OwnerOnly);
}

bool UPlayerProgressionComponent::IsServer() const
{
    return GetOwner() && GetOwner()->HasAuthority();
}

// ========== Weapon Mastery ==========
void UPlayerProgressionComponent::AddWeaponXP(EWeaponType Weapon, float XP)
{
    if (!IsServer()) return;

    // Anti‑exploit
    float Now = GetWorld()->GetTimeSeconds();
    float* LastTime = LastXPTime.Find(Weapon);
    if (LastTime && (Now - *LastTime) < WEAPON_XP_COOLDOWN)
        return;
    LastXPTime.Add(Weapon, Now);

    FWeaponMasteryData& Data = WeaponMastery.FindOrAdd(Weapon);
    Data.WeaponType = Weapon;
    Data.MasteryXP += XP;

    // Проверка на повышение уровня
    UWeaponMasterySubsystem* Sub = GetWorld()->GetSubsystem<UWeaponMasterySubsystem>();
    if (Sub)
    {
        float RequiredXP = Sub->GetRequiredXPForLevel(Data.MasteryLevel);
        while (Data.MasteryXP >= RequiredXP && Data.MasteryLevel < 10)
        {
            Data.MasteryXP -= RequiredXP;
            Data.MasteryLevel++;
            LevelUpWeapon(Weapon);
            RequiredXP = Sub->GetRequiredXPForLevel(Data.MasteryLevel);
        }
    }
}

int32 UPlayerProgressionComponent::GetWeaponMasteryLevel(EWeaponType Weapon) const
{
    if (const FWeaponMasteryData* Data = WeaponMastery.Find(Weapon))
        return Data->MasteryLevel;
    return 1;
}

TArray<int32> UPlayerProgressionComponent::GetUnlockedSkills(EWeaponType Weapon) const
{
    if (const FWeaponMasteryData* Data = WeaponMastery.Find(Weapon))
        return Data->UnlockedSkills;
    return TArray<int32>();
}

void UPlayerProgressionComponent::LevelUpWeapon(EWeaponType Weapon)
{
    USkillEvolutionSubsystem* Sub = GetWorld()->GetSubsystem<USkillEvolutionSubsystem>();
    if (Sub)
    {
        TArray<int32> NewSkills = Sub->GetSkillsForWeaponAndLevel(Weapon, GetWeaponMasteryLevel(Weapon));
        FWeaponMasteryData& Data = WeaponMastery.FindOrAdd(Weapon);
        for (int32 SkillID : NewSkills)
        {
            if (!Data.UnlockedSkills.Contains(SkillID))
            {
                Data.UnlockedSkills.Add(SkillID);
                // Автоматически добавляем эволюцию навыка
                FSkillEvolution Evolution;
                Evolution.SkillID = SkillID;
                Evolution.Level = 1;
                Evolution.DamageModifier = 1.f;
                Evolution.bUnlockedModifier = false;
                SkillEvolutions.Add(SkillID, Evolution);
            }
        }
    }
}

// ========== Skill Evolution ==========
void UPlayerProgressionComponent::UnlockSkill(int32 SkillID, EWeaponType Weapon)
{
    if (!IsServer()) return;
    FWeaponMasteryData& Data = WeaponMastery.FindOrAdd(Weapon);
    if (!Data.UnlockedSkills.Contains(SkillID))
    {
        Data.UnlockedSkills.Add(SkillID);
        FSkillEvolution Evolution;
        Evolution.SkillID = SkillID;
        Evolution.Level = 1;
        Evolution.DamageModifier = 1.f;
        Evolution.bUnlockedModifier = false;
        SkillEvolutions.Add(SkillID, Evolution);
    }
}

void UPlayerProgressionComponent::EvolveSkill(int32 SkillID, int32 NewLevel, int32 ChosenModifierID)
{
    if (!IsServer()) return;
    FSkillEvolution* Evol = SkillEvolutions.Find(SkillID);
    if (!Evol) return;
    Evol->Level = NewLevel;
    Evol->DamageModifier = 1.f + 0.1f * (NewLevel - 1); // пример
    if (NewLevel >= 3) Evol->bUnlockedModifier = true;
    if (ChosenModifierID != 0) Evol->ChosenModifierID = ChosenModifierID;
}

float UPlayerProgressionComponent::GetSkillDamageModifier(int32 SkillID) const
{
    if (const FSkillEvolution* Evol = SkillEvolutions.Find(SkillID))
        return Evol->DamageModifier;
    return 1.f;
}

// ========== Element Affinity ==========
void UPlayerProgressionComponent::AddElementXP(EElementType Element, float XP)
{
    if (!IsServer()) return;
    FElementAffinity& Data = ElementAffinities.FindOrAdd(Element);
    Data.Element = Element;
    Data.AffinityXP += XP;

    UElementAffinitySubsystem* Sub = GetWorld()->GetSubsystem<UElementAffinitySubsystem>();
    if (Sub)
    {
        float RequiredXP = Sub->GetRequiredXPForLevel(Data.Level);
        while (Data.AffinityXP >= RequiredXP && Data.Level < 4)
        {
            Data.AffinityXP -= RequiredXP;
            Data.Level++;
            LevelUpElement(Element);
            RequiredXP = Sub->GetRequiredXPForLevel(Data.Level);
        }
    }
}

int32 UPlayerProgressionComponent::GetElementAffinityLevel(EElementType Element) const
{
    if (const FElementAffinity* Data = ElementAffinities.Find(Element))
        return Data->Level;
    return 1;
}

void UPlayerProgressionComponent::LevelUpElement(EElementType Element)
{
    // Дополнительные эффекты
}

// ========== Crystal ==========
void UPlayerProgressionComponent::EquipCrystal(int32 CrystalID)
{
    if (!IsServer()) return;
    EquippedCrystalID = CrystalID;
}

void UPlayerProgressionComponent::UnequipCrystal()
{
    if (!IsServer()) return;
    EquippedCrystalID = -1;
}

FElementCrystal UPlayerProgressionComponent::GetEquippedCrystalData() const
{
    UCrystalSubsystem* Sub = GetWorld()->GetSubsystem<UCrystalSubsystem>();
    if (Sub && EquippedCrystalID != -1)
    {
        return Sub->GetCrystalData(EquippedCrystalID);
    }
    // Default
    FElementCrystal Default;
    Default.Element = EElementType::Fire;
    Default.Grade = ECrystalGrade::Common;
    Default.EffectMultiplier = 1.f;
    return Default;
}

// ========== Guild Career ==========
void UPlayerProgressionComponent::AddGuildProgress(float Progress)
{
    if (!IsServer()) return;
    GuildCareer.Progress += Progress;
    UGuildCareerSubsystem* Sub = GetWorld()->GetSubsystem<UGuildCareerSubsystem>();
    if (Sub)
    {
        float Required = Sub->GetRequiredProgressForRank(GuildCareer.Rank);
        while (GuildCareer.Progress >= Required && (int32)GuildCareer.Rank < (int32)EGuildRank::GuildLeader)
        {
            GuildCareer.Progress -= Required;
            GuildCareer.Rank = (EGuildRank)((int32)GuildCareer.Rank + 1);
            LevelUpGuild();
            Required = Sub->GetRequiredProgressForRank(GuildCareer.Rank);
        }
    }
}

void UPlayerProgressionComponent::ChangeGuild(EGuildType NewGuild)
{
    if (!IsServer()) return;
    // Штраф 50%
    GuildCareer.Progress *= 0.5f;
    GuildCareer.Guild = NewGuild;
    GuildCareer.Rank = EGuildRank::Recruit;
}

void UPlayerProgressionComponent::LevelUpGuild()
{
    // бонусы
}

// ========== Profession ==========
void UPlayerProgressionComponent::AddProfessionXP(int32 ProfessionID, float XP)
{
    if (!IsServer()) return;
    FProfessionData& Data = Professions.FindOrAdd(ProfessionID);
    Data.ProfessionID = ProfessionID;
    Data.XP += XP;

    UProfessionSubsystem* Sub = GetWorld()->GetSubsystem<UProfessionSubsystem>();
    if (Sub)
    {
        float Required = Sub->GetRequiredXPForProfessionLevel(Data.Level);
        while (Data.XP >= Required && Data.Level < 10)
        {
            Data.XP -= Required;
            Data.Level++;
            LevelUpProfession(ProfessionID);
            Required = Sub->GetRequiredXPForProfessionLevel(Data.Level);
        }
    }
}

int32 UPlayerProgressionComponent::GetProfessionLevel(int32 ProfessionID) const
{
    if (const FProfessionData* Data = Professions.Find(ProfessionID))
        return Data->Level;
    return 1;
}

void UPlayerProgressionComponent::LevelUpProfession(int32 ProfessionID)
{
    // открываются рецепты
}

// ========== Reputation ==========
void UPlayerProgressionComponent::AddReputationXP(int32 FactionID, float XP)
{
    if (!IsServer()) return;
    FReputationData& Data = Reputations.FindOrAdd(FactionID);
    Data.FactionID = FactionID;
    Data.ReputationXP += XP;
    Data.ReputationLevel = FMath::Max(Data.ReputationLevel, 1);

    UReputationSubsystem* Sub = GetWorld()->GetSubsystem<UReputationSubsystem>();
    if (Sub)
    {
        float Required = Sub->GetRequiredXPForReputationLevel(Data.ReputationLevel);
        while (Data.ReputationXP >= Required && Data.ReputationLevel < 10)
        {
            Data.ReputationXP -= Required;
            Data.ReputationLevel++;
            LevelUpReputation(FactionID);
            Required = Sub->GetRequiredXPForReputationLevel(Data.ReputationLevel);
        }
    }
}

int32 UPlayerProgressionComponent::GetReputationLevel(int32 FactionID) const
{
    if (const FReputationData* Data = Reputations.Find(FactionID))
        return Data->ReputationLevel;
    return 0;
}

void UPlayerProgressionComponent::LevelUpReputation(int32 FactionID)
{
    // доступ к новым квестам/предметам
}

// ========== Build ==========
void UPlayerProgressionComponent::SetActiveSkills(const TArray<int32>& SkillIDs)
{
    if (!IsServer()) return;
    if (SkillIDs.Num() > 6)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attempt to equip more than 6 skills"));
        return;
    }
    // Проверяем, что все навыки разблокированы
    for (int32 SkillID : SkillIDs)
    {
        if (!SkillEvolutions.Contains(SkillID))
        {
            UE_LOG(LogTemp, Warning, TEXT("Attempt to equip locked skill %d"), SkillID);
            return;
        }
    }
    ActiveSkillIDs = SkillIDs;
}
