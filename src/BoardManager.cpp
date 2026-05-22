#include "BoardManager.h"
#include <iostream>

BoardManager::BoardManager() = default;


BoardManager::~BoardManager() {
    for (Pigeon* p : activePigeons) {
        delete p;
    }
    activePigeons.clear();
}

void BoardManager::spawnBabyPigeon(int count)
{
    for (int i=1;i<=count;i++)
        activePigeons.push_back(new BabyPigeon());
}
void BoardManager::spawnMutantPigeon()
{
    activePigeons.push_back(new MutantPigeon());
}
void BoardManager::performMerge(int index1, int index2)
{
    if (index1 == index2 || index1 < 0 || index2 < 0 ||
        index1 >= static_cast<int>(activePigeons.size()) ||
        index2 >= static_cast<int>(activePigeons.size())) {
        std::cout << "Invalid merge selection!\n";
        return;
    }

    Pigeon* p1 = activePigeons[index1];
    Pigeon* p2 = activePigeons[index2];

    Pigeon* newPigeon = p1->merge(*p2);

    if (newPigeon != nullptr)
    {
        std::cout << "Evolved into a " << newPigeon->getName() << "\n";

        delete p1;
        delete p2;

        activePigeons[index1] = nullptr;
        activePigeons[index2] = nullptr;
        erase(activePigeons, nullptr);

        activePigeons.push_back(newPigeon);
    }
    else
        std::cout << "Merge failed. Those pigeons cannot be combined.\n";
}

int BoardManager::getTotalPigeonsAlive() const
{
    return activePigeons.size();
}

void BoardManager::printBoard() const
{
    std::cout << "\n CURRENT NEST\n";
    for (size_t i = 0; i < activePigeons.size(); ++i) {
        std::cout << "[" << i << "] " << activePigeons[i]->getName() << "\n";
    }
    std::cout << "Pigeons on board: " << getTotalPigeonsAlive() << "\n\n";
}
