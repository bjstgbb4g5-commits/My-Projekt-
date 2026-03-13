#include "MountBreedingSubsystem.h"

bool UMountBreedingSubsystem::CanBreed(const FMountData& ParentA, const FMountData& ParentB) const
{
    return ParentA.Species == ParentB.Species;
}

FMountData UMountBreedingSubsystem::BreedMount(const FMountData& ParentA, const FMountData& ParentB)
{
    FMountData Child;
    Child.Species = ParentA.Species;
    Child.Type = (FMath::RandBool()) ? ParentA.Type : ParentB.Type;
    Child.Rarity = (FMath::RandBool()) ? ParentA.Rarity : ParentB.Rarity;

    Child.Genes.SpeedGene = (ParentA.Genes.SpeedGene + ParentB.Genes.SpeedGene) * 0.5f;
    Child.Genes.StaminaGene = (ParentA.Genes.StaminaGene + ParentB.Genes.StaminaGene) * 0.5f;
    Child.Genes.StrengthGene = (ParentA.Genes.StrengthGene + ParentB.Genes.StrengthGene) * 0.5f;
    Child.Genes.CargoGene = (ParentA.Genes.CargoGene + ParentB.Genes.CargoGene) * 0.5f;
    Child.Genes.TemperGene = (ParentA.Genes.TemperGene + ParentB.Genes.TemperGene) * 0.5f;

    // Базовые статы по типу
    switch (Child.Type)
    {
    case EMountType::Travel:
        Child.Stats = {1300.f, 100.f, 1200.f, 50.f, 0.f};
        break;
    case EMountType::Combat:
        Child.Stats = {900.f, 100.f, 2000.f, 120.f, 0.f};
        break;
    case EMountType::Cargo:
        Child.Stats = {600.f, 100.f, 3000.f, 80.f, 500.f};
        break;
    }

    // Применяем гены (будут применены при спавне, здесь просто копируем)
    return Child;
}

int32 UMountBreedingSubsystem::GetBreedingCost(const FMountData& ParentA, const FMountData& ParentB) const
{
    int32 Base = 1000;
    int32 RarityBonus = (int32)ParentA.Rarity * 500 + (int32)ParentB.Rarity * 500;
    return Base + RarityBonus;
}
