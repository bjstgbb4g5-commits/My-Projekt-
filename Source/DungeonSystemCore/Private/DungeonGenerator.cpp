#include "DungeonGenerator.h"

TArray<FDungeonRoom> UDungeonGenerator::GenerateDungeon(int32 Seed, int32 NumRooms)
{
    TArray<FDungeonRoom> Rooms;
    if (NumRooms <= 0) return Rooms;

    // Примерная генерация: всегда вход, потом бой, элитка, босс
    Rooms.Add({EDungeonRoomType::Entrance, 0});
    for (int32 i = 1; i < NumRooms - 1; ++i)
    {
        if (i % 3 == 0)
            Rooms.Add({EDungeonRoomType::Elite, 1});
        else
            Rooms.Add({EDungeonRoomType::Combat, 6});
    }
    Rooms.Add({EDungeonRoomType::Boss, 1});
    return Rooms;
}
