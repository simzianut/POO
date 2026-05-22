#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H

#include <vector>
#include "Pigeon.h"
using namespace std;

class BoardManager {
private:
    vector<Pigeon*> activePigeons;

public:
    BoardManager();
    ~BoardManager();

    void spawnBabyPigeon(int count=1);
    void spawnMutantPigeon();
    void performMerge(int index1, int index2);
    [[nodiscard]] int getTotalPigeonsAlive() const;
    void printBoard() const;
};

#endif
