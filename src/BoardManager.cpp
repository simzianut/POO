#include "BoardManager.h"
#include "Crate.h"
#include "GameException.h"
#include <algorithm>
#include <iostream>
#include <string>

istream& operator>>(istream& in, BoardManager& board)
{
    const string commandName = board.expectedInputName.empty() ? "command" : board.expectedInputName;

    string token;
    if (!(in >> token))
        throw InvalidCommandException(commandName);

    size_t parsedCharacters = 0;
    int value = 0;
    try
    {
        value = stoi(token, &parsedCharacters);
    }
    catch (const exception&)
    {
        throw InvalidCommandException(token);
    }

    if (parsedCharacters != token.size())
        throw InvalidCommandException(token);

    board.lastInputCommand = value;
    board.expectedInputName.clear();
    return in;
}


BoardManager::BoardManager() :
    berryInventory{},
    coins(0),
    berryEffectDurationSeconds(15),
    lastInputCommand(0) {}

BoardManager::~BoardManager()
{
    for (const Pigeon* p : activePigeons)
        delete p;
    activePigeons.clear();
}

BoardManager& BoardManager :: getInstance()
{
    static BoardManager instance;
    return instance;
}

bool BoardManager::isValidPigeonIndex(const int index) const
{
    return index >= 0 && index < static_cast<int>(activePigeons.size());
}

int BoardManager::getBerryInventoryCount(BerryType berryType) const
{
    const int index = static_cast<int>(berryType);
    if (index <= 0 || index >= static_cast<int>(berryInventory.size()))
        return 0;

    return berryInventory[index];
}

bool BoardManager::hasDiscoveredLargestPigeon() const
{
    for (const Pigeon* pigeon : activePigeons)
        if (dynamic_cast<const Pigeostrich*>(pigeon) != nullptr)
            return true;

    return false;
}

void BoardManager::sortPigeonsByTier()
{
    stable_sort(activePigeons.begin(), activePigeons.end(), [](const Pigeon* left, const Pigeon* right) {
        if (left == nullptr)
            return false;
        if (right == nullptr)
            return true;
        if (left->getTier() == right->getTier())
            return left->getName() < right->getName();
        return left->getTier() < right->getTier();
    });
}

void BoardManager::openCrate()
{
    addPigeon(Crate::open(getBiggestPigeonTier()));
}

void BoardManager::addPigeon(Pigeon* pigeon)
{
    if (pigeon == nullptr)
        return;
    activePigeons.push_back(pigeon);
    update();
}

void BoardManager::spawnBabyPigeon(const int count)
{
    for (int i = 1; i <= count; i++)
        addPigeon(new BabyPigeon());
}

void BoardManager::spawnMutantPigeon()
{
    addPigeon(new MutantPigeon());
}

void BoardManager::performMerge(const int index1, const int index2)
{
    if (index1 == index2 || !isValidPigeonIndex(index1) || !isValidPigeonIndex(index2))
        throw InvalidCommandException("merge indices");

    Pigeon* p1 = activePigeons[index1];
    Pigeon* p2 = activePigeons[index2];

    if (!(*p1 == *p2))
    {
        cout << "Merge failed. Those pigeons cannot be combined.\n";
        return;
    }

    if (p1->hasActiveBerryEffect() && p2->hasActiveBerryEffect())
    {
        cout << "Only one active berry effect can exist at a time. Merge blocked.\n";
        return;
    }

    const Pigeon* berryPigeon = nullptr;
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
            int earned = static_cast<int>(5000.0f * berryPigeon->getPoopPerSecond());
            coins += earned;
            cout << "Yellow Berry bonus: +" << earned << " coins.\n";
        }

        delete p1;
        delete p2;

        activePigeons[index1] = nullptr;
        activePigeons[index2] = nullptr;
        erase(activePigeons, nullptr);

        activePigeons.push_back(newPigeon);

        update();
    }
    else
        cout << "Merge failed. Those pigeons cannot be combined.\n";
}

void BoardManager::performMerge(const int index)
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

int BoardManager::getBiggestPigeonTier() const
{
    int biggestTier = 0;
    for (const Pigeon* pigeon : activePigeons)
        if (pigeon != nullptr && pigeon->getTier() > biggestTier)
            biggestTier = pigeon->getTier();
    return biggestTier;
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

void BoardManager::printBoard()
{
    sortPigeonsByTier();
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
    if (hasDiscoveredLargestPigeon())
        cout << "Largest pigeon discovered: Pigeostrich\n\n";
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
        for (const Poop* poop : poops)
        {
            if (poop == nullptr)
                continue;
            const int earned = poop->collect();
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
    expectedInputName = "shop category";
    cin >> *this;
    const int category = lastInputCommand;
    if (category == 1)
    {
        shop.showPigeonCategory(*this);
        if (shop.getAvailablePigeonOffers(*this).empty())
            return;

        expectedInputName = "pigeon tier";
        cin >> *this;
        const int pigeonTier = lastInputCommand;
        buyNewPigeon(pigeonTier);
    }
    else if (category == 2)
    {
        shop.showBerryCategory();
        expectedInputName = "berry type";
        cin >> *this;
        const int berryType = lastInputCommand;
        buyNewBerry(berryType);
    }
    else
        throw InvalidCommandException(std::to_string(category));
}

void BoardManager::showFeedBerryMenu()
{
    printBoard();
    printBerryInventory();

    cout << "Choose berry type and pigeon index: ";
    expectedInputName = "berry type";
    cin >> *this;
    const int berryType = lastInputCommand;
    expectedInputName = "pigeon index";
    cin >> *this;
    const int pigeonIndex = lastInputCommand;
    feedBerry(berryType, pigeonIndex);
}

void BoardManager::buyNewPigeon(const int desiredPigeonTier)
{
    if (!shop.canBuyPigeon(*this, desiredPigeonTier))
        throw InvalidCommandException(std::to_string(desiredPigeonTier));

    const int price = shop.getPigeonPrice(desiredPigeonTier);
    if (coins < price)
        throw NotEnoughCoinsException(price, coins);

    Pigeon* pigeon = Pigeon::createByTier(desiredPigeonTier);
    if (pigeon == nullptr)
    {
        cout << "Could not create that pigeon.\n";
        return;
    }

    const string pigeonName = pigeon->getName();
    coins -= price;

    if (!shop.recordPigeonPurchase(desiredPigeonTier))
    {
        coins += price;
        delete pigeon;
        cout << "Could not update the shop after purchase.\n";
        return;
    }

    addPigeon(pigeon);

    cout << "Purchased " << pigeonName << " for " << price << " coins.\n";
    cout << "Next price: " << shop.getPigeonPrice(desiredPigeonTier) << " coins.\n";
}

void BoardManager::buyNewBerry(int desiredBerryType)
{
    BerryType berryType = Berry::typeFromInt(desiredBerryType);
    if (!shop.canBuyBerry(berryType))
        throw InvalidCommandException(std::to_string(desiredBerryType));

    const int price = shop.getBerryPrice(berryType);
    if (coins < price)
        throw NotEnoughCoinsException(price, coins);

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
        throw InvalidCommandException(std::to_string(desiredBerryType));

    if (getBerryInventoryCount(berryType) <= 0)
    {
        cout << "You do not own a " << Berry::getNameByType(berryType) << ".\n";
        return;
    }

    if (!isValidPigeonIndex(pigeonIndex))
        throw InvalidCommandException(std::to_string(pigeonIndex));

    if (hasAnyActiveBerryEffect())
        throw BerryAlreadyInUseException();

    Pigeon* pigeon = activePigeons[pigeonIndex];
    if (berryType == BerryType::Purple && dynamic_cast<const Pigeostrich*>(pigeon) != nullptr)
    {
        cout << "Pigeostrich is already the largest pigeon.\n";
        return;
    }

    pigeon->applyBerryEffect(berryType, berryEffectDurationSeconds);
    berryInventory[static_cast<int>(berryType)]--;

    cout << "Fed " << Berry::getNameByType(berryType)
         << " to " << pigeon->getName() << ".\n";
    cout << "Effect duration: " << berryEffectDurationSeconds << " seconds.\n";
}
