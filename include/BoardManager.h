#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H

#include <array>
#include <iosfwd>
#include <string>
#include <vector>
#include "Pigeon.h"
#include "Encyclopedia.h"
#include "Shop.h"

using namespace std;

class BoardManager {
private:
    vector<Pigeon*> activePigeons;
    array<int, 4> berryInventory;
    Encyclopedia encyclopedia;
    Shop shop;
    int coins;
    int berryEffectDurationSeconds;
    int lastInputCommand;
    string expectedInputName;

    [[nodiscard]] bool isValidPigeonIndex(int index) const;
    [[nodiscard]] int getBerryInventoryCount(BerryType berryType) const;
    [[nodiscard]] bool hasDiscoveredLargestPigeon() const;
    void sortPigeonsByTier();

    BoardManager();


public:
    friend istream& operator>>(istream& in, BoardManager& board);

    ~BoardManager();
    static BoardManager& getInstance();

    BoardManager(const BoardManager&) = delete;
    BoardManager &operator=(BoardManager) = delete;

    void openCrate();
    void addPigeon(Pigeon* pigeon);
    void spawnBabyPigeon(int count=1);
    void spawnMutantPigeon();
    void performMerge(int index1, int index2);
    void performMerge(int index);
    [[nodiscard]] int getTotalPigeonsAlive() const;
    [[nodiscard]] int getBiggestPigeonLevel() const;
    [[nodiscard]] int getBiggestPigeonTier() const;
    [[nodiscard]] int getCoins() const;
    [[nodiscard]] bool hasAnyActiveBerryEffect() const;
    void printBoard();
    void printBerryInventory() const;

    void update();

    void showEncyclopedia() const;
    void showShop();
    void showFeedBerryMenu();
    void buyNewPigeon(int desiredPigeonTier);
    void buyNewBerry(int desiredBerryType);
    void feedBerry(int desiredBerryType, int pigeonIndex);
};

#endif
