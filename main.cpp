#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <string>

#include "BoardManager.h"
#include "GameException.h"

using namespace std;

int main()
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

    while (1)
    {
        try
        {
            board.update();
            string command;
            cin >> command;

            if (command == "exit")
            {
                cout << "Goodbye!\n";
                return 0;
            }

            if (command == "1")
                board.openCrate();
            else if (command == "2")
            {
                board.printBoard();
                int index1, index2;
                if (!(cin >> index1 >> index2))
                    throw InvalidCommandException("merge indices");
                board.performMerge(index1, index2);
            }
            else if (command == "3")
                board.showEncyclopedia();
            else if (command == "4")
                board.showShop();
            else if (command == "5")
                board.printBoard();
            else if (command == "6")
                board.showFeedBerryMenu();
            else
                throw InvalidCommandException(command);
        }
        catch (const GameException& exception)
        {
            // Upcasting proof: derived game exceptions are handled through the base class reference.
            cout << exception.what() << "\n";
            cin.clear();
        }
        catch (const exception& exception)
        {
            cout << exception.what() << "\n";
            cin.clear();
        }
    }
}
