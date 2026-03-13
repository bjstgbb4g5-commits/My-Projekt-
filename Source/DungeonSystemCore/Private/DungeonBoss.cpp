#include "DungeonBoss.h"
#include "DungeonSubsystem.h"
#include "Engine/World.h"

void ADungeonBoss::OnBossKilled()
{
    UWorld* World = GetWorld();
    if (World && World->IsServer())
    {
        UDungeonSubsystem* DungeonSys = World->GetSubsystem<UDungeonSubsystem>();
        if (DungeonSys && DungeonInstanceID != -1)
        {
            // Уведомляем, что босс убит – можно завершить данж
            // В реальном проекте можно использовать отдельный метод для завершения данжа
        }
    }
}
