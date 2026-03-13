#pragma once

#include "ChronicleTypes.h"

inline ESeasonType GetSeasonTypeByIndex(int32 SeasonIndex)
{
    switch (SeasonIndex)
    {
    case 1:  return ESeasonType::ApostleWar;
    case 2:  return ESeasonType::ApostlePlague;
    case 3:  return ESeasonType::ApostleVoid;
    case 4:  return ESeasonType::ApostleStorm;
    case 5:  return ESeasonType::ApostleFlame;
    case 6:  return ESeasonType::NoxSeason;
    default: return ESeasonType::ApostleWar;
    }
}
