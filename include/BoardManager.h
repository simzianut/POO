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
    Encyclopedia encyclopedia;
    Shop shop;

public:
    BoardManager();
    ~BoardManager();

    void openCrate();
    void addPigeon(Pigeon* pigeon);
    void spawnBabyPigeon(int count=1);//test
    void spawnMutantPigeon();
    void performMerge(int index1, int index2);
    [[nodiscard]] int getTotalPigeonsAlive() const;
    [[nodiscard]] int getBiggestPigeonLevel() const;
    void printBoard() const;
    void createPoop() const;

    void showEncyclopedia() const;
    void showShop();
    void buyNewPigeon(int desiredPigeonLevel);

};

#endif