#include "BoardManager.h"
#include "Crate.h"
#include <iostream>


BoardManager::BoardManager() :
    berryInventory(4, 0),
    coins(0),
    berryEffectDurationSeconds(15) {}

BoardManager::~BoardManager()
{
    for (Pigeon* p : activePigeons)
        delete p;
    activePigeons.clear();
}

bool BoardManager::isValidPigeonIndex(int index) const
{
    return index >= 0 && index < static_cast<int>(activePigeons.size());
}

int BoardManager::getBerryInventoryCount(BerryType berryType) const
{
    int index = static_cast<int>(berryType);
    if (index <= 0 || index >= static_cast<int>(berryInventory.size()))
        return 0;

    return berryInventory[index];
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
    if (index1 == index2 || !isValidPigeonIndex(index1) || !isValidPigeonIndex(index2))
    {
        cout << "Invalid merge selection!\n";
        return;
    }

    Pigeon* p1 = activePigeons[index1];
    Pigeon* p2 = activePigeons[index2];

    if (p1->hasActiveBerryEffect() && p2->hasActiveBerryEffect())
    {
        cout << "Only one active berry effect can exist at a time. Merge blocked.\n";
        return;
    }

    Pigeon* berryPigeon = nullptr;
    if (p1->hasActiveBerryEffect())
        berryPigeon = p1;
    else if (p2->hasActiveBerryEffect())
        berryPigeon = p2;

    Pigeon* newPigeon = p1->merge(*p2);

    if (newPigeon != nullptr)
    {
        cout << "Evolved into a " << newPigeon->getName() << "!\n";

        if (berryPigeon != nullptr && berryPigeon->hasActiveBerryEffect(BerryType::Yellow))
        {
            int earned = static_cast<int>(5.0f * berryPigeon->getPoopPerSecond());
            coins += earned;
            cout << "Yellow Berry bonus: +" << earned << " coins.\n";
        }

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

void BoardManager::performMerge(int index)
{
    if (!isValidPigeonIndex(index))
    {
        cout << "Invalid pigeon selection!\n";
        return;
    }

    Pigeon* pigeon = activePigeons[index];
    if (!pigeon->hasBerryEffect() || pigeon->getActiveBerryType() != BerryType::Purple ||
        !pigeon->isBerryEffectExpired())
    {
        cout << "Purple Berry effect is not ready for this pigeon.\n";
        return;
    }

    Pigeon* newPigeon = pigeon->merge(*pigeon);
    if (newPigeon == nullptr)
    {
        cout << "Purple Berry expired, but " << pigeon->getName() << " cannot evolve further.\n";
        pigeon->clearBerryEffect();
        return;
    }

    cout << "Purple Berry evolved " << pigeon->getName()
         << " into a " << newPigeon->getName() << "!\n";

    delete pigeon;
    activePigeons[index] = newPigeon;

    encyclopedia.updateEncyclopedia(
        newPigeon->getName(),
        newPigeon->getPoopPerSecond(),
        newPigeon->getDescription()
    );
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

bool BoardManager::hasAnyActiveBerryEffect() const
{
    for (const Pigeon* pigeon : activePigeons)
        if (pigeon != nullptr && pigeon->hasActiveBerryEffect())
            return true;

    return false;
}

void BoardManager::printBoard() const
{
    cout << "         CURRENT NEST\n";
    for (size_t i = 0; i < activePigeons.size(); ++i)
    {
        cout << "index [" << i << "] " << activePigeons[i]->getName();
        if (activePigeons[i]->hasBerryEffect())
        {
            cout << " | " << Berry::getNameByType(activePigeons[i]->getActiveBerryType())
                 << " effect: " << activePigeons[i]->getRemainingBerrySeconds()
                 << "s left";
        }
        cout << "\n";
    }
    cout << "Pigeons on board: " << getTotalPigeonsAlive() << "\n";
    cout << "Coins: " << coins << "\n\n";
}

void BoardManager::printBerryInventory() const
{
    cout << "         BERRY INVENTORY\n";
    cout << "1 - " << Berry::getNameByType(BerryType::Red)
         << ": " << getBerryInventoryCount(BerryType::Red) << "\n";
    cout << "2 - " << Berry::getNameByType(BerryType::Yellow)
         << ": " << getBerryInventoryCount(BerryType::Yellow) << "\n";
    cout << "3 - " << Berry::getNameByType(BerryType::Purple)
         << ": " << getBerryInventoryCount(BerryType::Purple) << "\n\n";
}


void BoardManager::update()
{
    for (size_t i = 0; i < activePigeons.size(); ++i)
    {
        Pigeon* pigeon = activePigeons[i];
        if (pigeon == nullptr)
            continue;

        encyclopedia.updateEncyclopedia(
            pigeon->getName(),
            pigeon->getPoopPerSecond(),
            pigeon->getDescription()
        );

        if (pigeon->hasBerryEffect() &&
            pigeon->getActiveBerryType() == BerryType::Purple &&
            pigeon->isBerryEffectExpired())
        {
            performMerge(static_cast<int>(i));
            continue;
        }

        vector<Poop*> poops = pigeon->dropPoopsIfReady();
        for (Poop* poop : poops)
        {
            if (poop == nullptr)
                continue;
            int earned = poop->collect();
            coins += earned;
            delete poop;
        }

        if (pigeon->isBerryEffectExpired())
        {
            cout << Berry::getNameByType(pigeon->getActiveBerryType())
                 << " effect ended on " << pigeon->getName() << ".\n";
            pigeon->clearBerryEffect();
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
        int berryType;
        cin >> berryType;
        buyNewBerry(berryType);
    }
}

void BoardManager::showFeedBerryMenu()
{
    printBoard();
    printBerryInventory();

    cout << "Choose berry type and pigeon index: ";
    int berryType, pigeonIndex;
    cin >> berryType >> pigeonIndex;
    feedBerry(berryType, pigeonIndex);
}

void BoardManager::buyNewPigeon(int desiredPigeonLevel)
{
    if (!shop.canBuyPigeon(*this, desiredPigeonLevel))
    {
        cout << "That pigeon is not available in the shop.\n";
        return;
    }

    int price = shop.getPigeonPrice(desiredPigeonLevel);
    if (coins < price)
    {
        cout << "Not enough coins. Price: " << price
             << " coins, your coins: " << coins << ".\n";
        return;
    }

    Pigeon* pigeon = Pigeon::createByLevel(desiredPigeonLevel);
    if (pigeon == nullptr)
    {
        cout << "Could not create that pigeon.\n";
        return;
    }

    string pigeonName = pigeon->getName();
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

void BoardManager::buyNewBerry(int desiredBerryType)
{
    BerryType berryType = Berry::typeFromInt(desiredBerryType);
    if (!shop.canBuyBerry(berryType))
    {
        cout << "That berry is not available in the shop.\n";
        return;
    }

    int price = shop.getBerryPrice(berryType);
    if (coins < price)
    {
        cout << "Not enough coins. Price: " << price
             << " coins, your coins: " << coins << ".\n";
        return;
    }

    coins -= price;
    berryInventory[static_cast<int>(berryType)]++;

    cout << "Purchased " << Berry::getNameByType(berryType)
         << " for " << price << " coins.\n";
    cout << "Owned: " << getBerryInventoryCount(berryType) << "\n";
}

void BoardManager::feedBerry(const int desiredBerryType, const int pigeonIndex)
{
    BerryType berryType = Berry::typeFromInt(desiredBerryType);
    if (!Berry::isValidType(berryType))
    {
        cout << "Invalid berry selection.\n";
        return;
    }

    if (getBerryInventoryCount(berryType) <= 0)
    {
        cout << "You do not own a " << Berry::getNameByType(berryType) << ".\n";
        return;
    }

    if (!isValidPigeonIndex(pigeonIndex))
    {
        cout << "Invalid pigeon selection.\n";
        return;
    }

    if (hasAnyActiveBerryEffect())
    {
        cout << "A berry effect is already active. Wait until it ends before feeding another berry.\n";
        return;
    }

    Pigeon* pigeon = activePigeons[pigeonIndex];
    pigeon->applyBerryEffect(berryType, berryEffectDurationSeconds);
    berryInventory[static_cast<int>(berryType)]--;

    cout << "Fed " << Berry::getNameByType(berryType)
         << " to " << pigeon->getName() << ".\n";
    cout << "Effect duration: " << berryEffectDurationSeconds << " seconds.\n";
}
