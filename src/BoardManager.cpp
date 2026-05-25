#include "BoardManager.h"
#include "Crate.h"
#include <iostream>


BoardManager::BoardManager() : coins(0) {}

BoardManager::~BoardManager()
{
    for (Pigeon* p : activePigeons)
        delete p;
    activePigeons.clear();
}

void BoardManager::openCrate()
{
    Crate crate;
    addPigeon(crate.open());
}

void BoardManager::addPigeon(Pigeon* pigeon)
{
    if (pigeon == nullptr)
        return;
    activePigeons.push_back(pigeon);
    update();// update ency
}

void BoardManager::spawnBabyPigeon(int count)
{
    for (int i = 1; i <= count; i++)
        addPigeon(new BabyPigeon());
}

void BoardManager::spawnMutantPigeon()
{
    addPigeon(new MutantPigeon());
}

void BoardManager::performMerge(int index1, int index2)
{
    if (index1 == index2 || index1 < 0 || index2 < 0 ||
        index1 >= static_cast<int>(activePigeons.size()) ||
        index2 >= static_cast<int>(activePigeons.size()))
    {
        cout << "Invalid merge selection!\n";
        return;
    }

    Pigeon* p1 = activePigeons[index1];
    Pigeon* p2 = activePigeons[index2];

    Pigeon* newPigeon = p1->merge(*p2);

    if (newPigeon != nullptr)
    {
        cout << "Evolved into a " << newPigeon->getName() << "!\n";

        delete p1;
        delete p2;

        activePigeons[index1] = nullptr;
        activePigeons[index2] = nullptr;
        erase(activePigeons, nullptr);

        activePigeons.push_back(newPigeon);

        update(); // enc
    }
    else
        cout << "Merge failed. Those pigeons cannot be combined.\n";
}



int BoardManager::getTotalPigeonsAlive() const
{
    return static_cast<int>(activePigeons.size());
}

int BoardManager::getBiggestPigeonLevel() const
{
    int biggestLevel = 0;
    for (const Pigeon* pigeon : activePigeons)
        if (pigeon != nullptr && pigeon->getLevel() > biggestLevel)
            biggestLevel = pigeon->getLevel();
    return biggestLevel;
}

int BoardManager::getCoins() const
{
    return coins;
}

void BoardManager::printBoard() const
{
    cout << "         CURRENT NEST\n";
    for (size_t i = 0; i < activePigeons.size(); ++i)
    {
        cout << "index [" << i << "] " << activePigeons[i]->getName() << "\n";
    }
    cout << "Pigeons on board: " << getTotalPigeonsAlive() << "\n";
    cout << "Coins: " << coins << "\n\n";
}


void BoardManager::update()
{
    for (Pigeon* pigeon : activePigeons)
    {
        if (pigeon == nullptr)
            continue;

        encyclopedia.updateEncyclopedia(
            pigeon->getName(),
            pigeon->getPoopPerSecond(),
            pigeon->getDescription()
        );

        Poop* poop = pigeon->dropPoopIfReady();
        if (poop != nullptr)
        {
            const int earned = poop->collect();
            coins += earned;
            delete poop;
        }
    }
}

void BoardManager::showEncyclopedia() const
{
    encyclopedia.showAll();
}

void BoardManager::showShop()
{
    shop.showCategories();
    int category;
    cout<<"Choose a category\n";
    cin >> category;
    if (category == 1)
    {
        shop.showPigeonCategory(*this);
        if (shop.getAvailablePigeonOffers(*this).empty())
            return;

        int pigeonLevel;
        cin >> pigeonLevel;
        buyNewPigeon(pigeonLevel);
    }
    else if (category == 2)
    {
        shop.showBerryCategory();
    }
}

void BoardManager::buyNewPigeon(int desiredPigeonLevel)
{
    if (!shop.canBuyPigeon(*this, desiredPigeonLevel))
    {
        cout << "That pigeon is not available in the shop.\n";
        return;
    }

    const int price = shop.getPigeonPrice(desiredPigeonLevel);
    if (coins < price)
    {
        cout << "Not enough coins. Price: " << price << " coins, your coins: " << coins << ".\n";
        return;
    }

    Pigeon* pigeon = Pigeon::createByLevel(desiredPigeonLevel);
    if (pigeon == nullptr)
    {
        cout << "Could not create that pigeon.\n";
        return;
    }

    const string pigeonName = pigeon->getName();
    coins -= price;

    if (!shop.recordPigeonPurchase(desiredPigeonLevel))
    {
        coins += price;
        delete pigeon;
        cout << "Could not update the shop after purchase.\n";
        return;
    }

    addPigeon(pigeon);

    cout << "Purchased " << pigeonName << " for " << price << " coins.\n";
    cout << "Next price: " << shop.getPigeonPrice(desiredPigeonLevel) << " coins.\n";
}
