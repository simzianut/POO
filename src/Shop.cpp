#include "Shop.h"
#include <iostream>

#include "BoardManager.h"

using namespace std;

Shop::Shop() :
    pigeonOffers({
        {1, "Baby Pigeon", 15, 0},
        {2, "Normal Pigeon", 40, 0},
        {3, "Chunky Pigeon", 150, 0},
        {4, "Fat Pigeon", 425, 0},
        {5, "Obese Pigeon", 1000, 0},
        {6, "Mutant Pigeon", 2500, 0}
    }) {}

const PigeonOffer* Shop::findPigeonOffer(const int pigeonLevel) const
{
    for (const PigeonOffer& offer : pigeonOffers)
        if (offer.level == pigeonLevel)
            return &offer;
    return nullptr;
}

PigeonOffer* Shop::findPigeonOffer(const int pigeonLevel)
{
    for (PigeonOffer& offer : pigeonOffers)
        if (offer.level == pigeonLevel)
            return &offer;
    return nullptr;
}

void Shop::showCategories()
{
    cout << "\n         SHOP\n";
    cout << "1. Pigeons\n";
    cout << "2. Berries\n";
}

void Shop::showPigeonCategory(const BoardManager& board) const
{
    const vector<PigeonOffer> availableOffers = getAvailablePigeonOffers(board);

    if (availableOffers.empty())
    {
        cout << "No pigeons are available in the shop yet.\n";
        return;
    }

    cout << "\n       PIGEON SHOP\n";
    for (const PigeonOffer& offer : availableOffers)
    {
        cout << offer.level << " - " << offer.name << " | price: " << offer.currentPrice << " coins | bought: " << offer.timesBought << " times\n";
    }
    cout << "Choose a pigeon level to buy: ";
}

void Shop::showBerryCategory()
{
    cout << "Berry shop category\n";
}

vector<PigeonOffer> Shop::getAvailablePigeonOffers(const BoardManager& board) const
{
    vector<PigeonOffer> availableOffers;

    for (const PigeonOffer& offer : pigeonOffers)
        if (canBuyPigeon(board, offer.level))
            availableOffers.push_back(offer);

    return availableOffers;
}

bool Shop::canBuyPigeon(const BoardManager& board, const int pigeonLevel) const
{
    if (findPigeonOffer(pigeonLevel) == nullptr)
        return false;

    return board.getBiggestPigeonLevel() - pigeonLevel >= 3;
}

int Shop::getPigeonPrice(const int pigeonLevel) const
{
    const PigeonOffer* offer = findPigeonOffer(pigeonLevel);
    if (offer == nullptr)
        return 0;

    return offer->currentPrice;
}

int Shop::getPigeonTimesBought(const int pigeonLevel) const
{
    const PigeonOffer* offer = findPigeonOffer(pigeonLevel);
    if (offer == nullptr)
        return 0;

    return offer->timesBought;
}

bool Shop::recordPigeonPurchase(const int pigeonLevel)
{
    PigeonOffer* offer = findPigeonOffer(pigeonLevel);
    if (offer == nullptr)
        return false;

    offer->timesBought++;
    offer->currentPrice = offer->currentPrice * 14 / 10;
    return true;
}
