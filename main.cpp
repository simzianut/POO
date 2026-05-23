#include "BoardManager.h"

int main() {
    BoardManager boardManager;
    boardManager.openCrate();
    boardManager.openCrate();
    boardManager.openCrate();boardManager.openCrate();boardManager.openCrate();boardManager.openCrate();boardManager.openCrate();
    boardManager.printBoard();
    boardManager.performMerge(0,1);
    boardManager.performMerge(0,1);
    boardManager.performMerge(0,1);
    boardManager.printBoard();
    boardManager.performMerge(2,1);

    boardManager.printBoard();
    boardManager.showEncyclopedia();

    return 0;
}
