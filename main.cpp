#include <iostream>
#include <array>

#include "BoardManager.h"


int main() {
    BoardManager boardManager;
    boardManager.spawnBabyPigeon(3);
    boardManager.spawnMutantPigeon();
    boardManager.spawnMutantPigeon();
    boardManager.printBoard();
    boardManager.performMerge(1,2);
    boardManager.printBoard();
    boardManager.performMerge(1,2);
    boardManager.printBoard();

    return 0;
}
