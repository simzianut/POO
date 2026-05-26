#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H

#include <vector>
#include "Pigeon.h"
#include "Encyclopedia.h"
#include "Shop.h"

using namespace std;

class BoardManager {
private:
    vector<Pigeon*> activePigeons;
    vector<int> berryInventory;
    Encyclopedia encyclopedia;
    Shop shop;
    int coins;
    int berryEffectDurationSeconds;

    [[nodiscard]] bool isValidPigeonIndex(int index) const;
    [[nodiscard]] int getBerryInventoryCount(BerryType berryType) const;

public:
    BoardManager();
    ~BoardManager();

    void openCrate();
    void addPigeon(Pigeon* pigeon);
    void spawnBabyPigeon(int count=1); // test
    void spawnMutantPigeon();          //test
    void performMerge(int index1, int index2);
    void performMerge(int index);
    [[nodiscard]] int getTotalPigeonsAlive() const;
    [[nodiscard]] int getBiggestPigeonLevel() const;
    [[nodiscard]] int getCoins() const;
    [[nodiscard]] bool hasAnyActiveBerryEffect() const;
    void printBoard() const;
    void printBerryInventory() const;

    void update();

    void showEncyclopedia() const;
    void showShop();
    void showFeedBerryMenu();
    void buyNewPigeon(int desiredPigeonLevel);
    void buyNewBerry(int desiredBerryType);
    void feedBerry(int desiredBerryType, int pigeonIndex);
};

#endif
