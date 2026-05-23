#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

#include "BoardManager.h"

using namespace std;

int main()
{
    srand(static_cast<unsigned>(std::time(nullptr)));

    BoardManager board;

    cout << "    PIGEON EVOLUTION - MENU\n";
    cout << "1 - Open crate\n";
    cout << "2 - Merge pigeons\n";
    cout << "3 - Show encyclopedia\n";
    cout << "4 - Open shop\n";
    cout << "5 - Print board & coins\n";
    cout << "Press anything - Quit\n\n";

    while (1)
    {
        board.update();
        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            board.openCrate();
            break;
        case 2:
            board.printBoard();
            {
                int index1, index2;
                cin >> index1 >> index2;
                board.performMerge(index1, index2);
            }
            break;
        case 3:
            board.showEncyclopedia();
            break;
        case 4:
            board.showShop();
            break;
        case 5:
            board.printBoard();
            break;
        default:
            cout << "Goodbye!\n";
            return 0;
        }
    }
}
