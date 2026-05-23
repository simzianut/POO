#include "Shop.h"
#include <iostream>

#include "Pigeon.h"
#include "BoardManager.h"

using namespace std;

void Shop::showCategories() const
{
    cout << "1. Pigeons\n";
    cout << "2. Berries\n";
}

void Shop::showPigeonCategory() const
{
    cout << "Pigeon shop category\n";
}

void Shop::showBerryCategory() const
{
    cout << "Berry shop category\n";
}


bool Shop::canBuyPigeon(const BoardManager& board, int pigeonLevel) const
{
    if (pigeonLevel < 1 || pigeonLevel > 6)
        return false;

    return board.getBiggestPigeonLevel() - pigeonLevel >= 3;
}

bool Shop::buyPigeon(BoardManager& board, int pigeonLevel) const
{
    if (!canBuyPigeon(board, pigeonLevel))
        return false;

    Pigeon* pigeon = Pigeon::createByLevel(pigeonLevel);
    if (pigeon == nullptr)
        return false;

    board.addPigeon(pigeon);
    return true;
}