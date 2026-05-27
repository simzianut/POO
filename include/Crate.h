#ifndef CRATE_H
#define CRATE_H

#include "Pigeon.h"

class Crate {
private:
    static constexpr int normalPigeonDropChancePercent = 20;

    [[nodiscard]] static bool canDropNormalPigeon(int biggestPigeonTierOnBoard);
    [[nodiscard]] static int choosePigeonTier(int biggestPigeonTierOnBoard);

public:
    [[nodiscard]] static Pigeon* open(int biggestPigeonTierOnBoard);
};

#endif
