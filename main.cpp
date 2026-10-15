#include <ctime>
#include <cstring>
#include "GameWindow.h"
#include "BoardManager.h"

int main(int argc, char* argv[])
{
    if (argc == 2 && std::strcmp(argv[1], "--smoke-test") == 0) {
        BoardManager& board = BoardManager::getInstance();
        board.spawnBabyPigeon();
        board.update();
        return board.getTotalPigeonsAlive() == 1 ? 0 : 1;
    }

    srand(static_cast<unsigned>(time(nullptr)));
    GameWindow window;
    window.run();
    return 0;
}

/* ─── Original console-based main (preserved for reference) ────────────────
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <limits>
#include <string>
#include "BoardManager.h"
#include "GameException.h"
using namespace std;
int main_console()
{
    srand(static_cast<unsigned>(time(nullptr)));
    BoardManager& board = BoardManager::getInstance();
    cout << "    PIGEON EVOLUTION - MENU\n";
    cout << "1 - Open crate\n";
    cout << "2 - Merge pigeons\n";
    cout << "3 - Show encyclopedia\n";
    cout << "4 - Open shop\n";
    cout << "5 - Print board & coins\n";
    cout << "6 - Feed berry\n";
    cout << "Type exit - Quit\n\n";
    while (true) {
        try {
            board.update();
            string command;
            if (!(cin >> command)) {
                if (cin.eof()) { cout << "Goodbye!\n"; return 0; }
                cin.clear();
                cout << "Invalid command. Type exit to quit.\n";
                continue;
            }
            if (command == "exit") { cout << "Goodbye!\n"; return 0; }
            if      (command == "1") board.openCrate();
            else if (command == "2") {
                board.printBoard();
                int i1, i2;
                if (!(cin >> i1 >> i2)) {
                    if (cin.eof()) { cout << "Goodbye!\n"; return 0; }
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid indices.\n"; continue;
                }
                board.performMerge(i1, i2);
            }
            else if (command == "3") board.showEncyclopedia();
            else if (command == "4") board.showShop();
            else if (command == "5") board.printBoard();
            else if (command == "6") board.showFeedBerryMenu();
            else cout << "Invalid command: " << command << ". Type exit to quit.\n";
        }
        catch (const exception& e) { cout << e.what() << "\n"; cin.clear(); }
    }
}
─────────────────────────────────────────────────────────────────────────── */
