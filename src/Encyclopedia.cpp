#include "Encyclopedia.h"

#include <iostream>

#include "BoardManager.h"

Encyclopedia::Encyclopedia()
{
    pigeons.push_back({"Baby Pigeon", 1, "Cute baby"});
    pigeons.push_back({"Normal Pigeon", 2, "Your normal street pigeon"});
    pigeons.push_back({"Chunky Pigeon", 5, "Ate too many peanuts"});
    pigeons.push_back({"Fat Pigeon", 10, "He just loves ice cream"});
    pigeons.push_back({"Obese Pigeon", 30, ""});
    pigeons.push_back({"Mutant Pigeon", 100, ""});
}

vector<PigeonInfo> Encyclopedia::getPigeons() const
{
    return pigeons;
}

// void Encyclopedia::showPigeons(BoardManager x) const
// {
//     for (size_t i = 0; i < pigeons.size(); ++i)
//         std::cout << "[" << i + 1 << "] " << pigeons[i].name << "\n";
// }

void Encyclopedia::showPigeonInfo(int level) const
{
    if (level < 1 || level > static_cast<int>(pigeons.size()))
    {
        std::cout << "No encyclopedia entry for this pigeon.\n";
        return;
    }

    const PigeonInfo& pigeon = pigeons[level - 1];
    std::cout << pigeon.name << "\n";
    std::cout << "Poop rate: " << pigeon.poopRate << "\n";
    std::cout << "Description: " << pigeon.description << "\n";
}
