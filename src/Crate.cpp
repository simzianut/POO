#include "Crate.h"

#include <cstdlib>

bool Crate::canDropNormalPigeon(const int biggestPigeonTierOnBoard)
{
    return biggestPigeonTierOnBoard >= 2;
}

int Crate::choosePigeonTier(const int biggestPigeonTierOnBoard)
{
    if (canDropNormalPigeon(biggestPigeonTierOnBoard) && rand() % 100 < normalPigeonDropChancePercent)
        return 2;

    return 1;
}

Pigeon* Crate::open(const int biggestPigeonTierOnBoard)
{
    return Pigeon::createByTier(choosePigeonTier(biggestPigeonTierOnBoard));
}
