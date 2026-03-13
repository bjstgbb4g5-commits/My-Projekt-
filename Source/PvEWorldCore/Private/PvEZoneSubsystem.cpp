#include "PvEZoneSubsystem.h"

EPvEZoneType UPvEZoneSubsystem::GetZoneTypeAtLocation(FVector Location) const
{
    // Заглушка: по расстоянию от центра
    float Dist = Location.Size2D();
    if (Dist > 40000) return EPvEZoneType::Black;
    if (Dist > 30000) return EPvEZoneType::Red;
    if (Dist > 20000) return EPvEZoneType::Blue;
    return EPvEZoneType::Green;
}
