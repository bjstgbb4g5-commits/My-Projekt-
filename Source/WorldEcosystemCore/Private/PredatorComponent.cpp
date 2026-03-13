#include "PredatorComponent.h"

UPredatorComponent::UPredatorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UPredatorComponent::SearchPrey()
{
    // Заглушка: поиск жертвы
}

void UPredatorComponent::AttackPrey(AActor* Target)
{
    // Заглушка: атака
}
