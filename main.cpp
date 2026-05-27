#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <limits>
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

    while (true)
    {
        try
        {
            board.update();
            string command;
            if (!(cin >> command))
            {
                if (cin.eof())
                {
                    cout << "Goodbye!\n";
                    return 0;
                }
                cin.clear();
                cout << "Invalid command: command. Type exit to quit.\n";
                continue;
            }

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
                {
                    if (cin.eof())
                    {
                        cout << "Goodbye!\n";
                        return 0;
                    }
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid command: merge indices. Type exit to quit.\n";
                    continue;
                }
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
                cout << "Invalid command: " << command << ". Type exit to quit.\n";
        }
        catch (const GameException& exception)
        {
            cout << exception.what() << "\n";
            if (cin.eof())
            {
                cout << "Goodbye!\n";
                return 0;
            }
            cin.clear();
        }
        catch (const exception& exception)
        {
            cout << exception.what() << "\n";
            if (cin.eof())
            {
                cout << "Goodbye!\n";
                return 0;
            }
            cin.clear();
        }
    }
}
