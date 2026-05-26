#include "Shop.h"
#include <iostream>

#include "Pigeon.h"
#include "BoardManager.h"

using namespace std;

Shop::Shop() :
    pigeonOffers({
        {1, "Baby Pigeon", 15, 0},
        {2, "Normal Pigeon", 40, 0},
        {3, "Chunky Pigeon", 150, 0},
        {4, "Fat Pigeon", 425, 0},
        {5, "Obese Pigeon", 1000, 0},
        {6, "Mutant Pigeon", 2500, 0},
        {7, "Pigeonworm", 3500, 0},
        {8, "Spingeon", 5000, 0},
        {9, "Cheerlegeon", 6900, 0},
        {10, "Chickenigeon", 9500, 0},
        {11, "Twingeon", 13500, 0},
        {12, "Pidgeknowledge", 18000, 0},
        {13, "Pidgeyes", 26000, 0},
    }),
    berryOffers({
        {BerryType::Red, Berry::getNameByType(BerryType::Red), 100},
        {BerryType::Yellow, Berry::getNameByType(BerryType::Yellow), 100},
        {BerryType::Purple, Berry::getNameByType(BerryType::Purple), 100}
    }) {}
const PigeonOffer* Shop::findPigeonOffer(const int pigeonTier) const
{
    for (const PigeonOffer& offer : pigeonOffers)
        if (offer.tier == pigeonTier)
            return &offer;
    return nullptr;
}

PigeonOffer* Shop::findPigeonOffer(const int pigeonTier)
{
    for (PigeonOffer& offer : pigeonOffers)
        if (offer.tier == pigeonTier)
            return &offer;
    return nullptr;
}

const BerryOffer* Shop::findBerryOffer(const BerryType berryType) const
{
    for (const BerryOffer& offer : berryOffers)
        if (offer.type == berryType)
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

    cout << "\n         PIGEON SHOP\n";
    for (const PigeonOffer& offer : availableOffers)
    {
        cout << offer.tier << " - " << offer.name
             << " | price: " << offer.currentPrice << " coins"
             << " | bought: " << offer.timesBought << " times\n";
    }
    cout << "Choose a pigeon tier to buy: ";
}

void Shop::showBerryCategory() const
{
    cout << "\n         BERRY SHOP\n";
    for (const BerryOffer& offer : berryOffers)
    {
        cout << static_cast<int>(offer.type) << " - " << offer.name
             << " | price: " << offer.price << " coins\n";
    }
    cout << "Choose a berry type to buy: ";
}

vector<PigeonOffer> Shop::getAvailablePigeonOffers(const BoardManager& board) const
{
    vector<PigeonOffer> availableOffers;

    for (const PigeonOffer& offer : pigeonOffers)
        if (canBuyPigeon(board, offer.tier))
            availableOffers.push_back(offer);

    return availableOffers;
}

bool Shop::canBuyPigeon(const BoardManager& board, const int pigeonTier) const
{
    if (findPigeonOffer(pigeonTier) == nullptr)
        return false;

    return board.getBiggestPigeonTier() - pigeonTier >= 3;
}

int Shop::getPigeonPrice(const int pigeonTier) const
{
    const PigeonOffer* offer = findPigeonOffer(pigeonTier);
    if (offer == nullptr)
        return 0;

    return offer->currentPrice;
}

int Shop::getPigeonTimesBought(const int pigeonTier) const
{
    const PigeonOffer* offer = findPigeonOffer(pigeonTier);
    if (offer == nullptr)
        return 0;

    return offer->timesBought;
}

bool Shop::recordPigeonPurchase(const int pigeonTier)
{
    PigeonOffer* offer = findPigeonOffer(pigeonTier);
    if (offer == nullptr)
        return false;

    offer->timesBought++;
    offer->currentPrice = offer->currentPrice * 14 / 10;
    return true;
}

vector<BerryOffer> Shop::getBerryOffers() const
{
    return berryOffers;
}

bool Shop::canBuyBerry(const BerryType berryType) const
{
    return findBerryOffer(berryType) != nullptr;
}

int Shop::getBerryPrice(const BerryType berryType) const
{
    const BerryOffer* offer = findBerryOffer(berryType);
    if (offer == nullptr)
        return 0;

    return offer->price;
}
