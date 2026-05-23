#include "Encyclopedia.h"
#include <iostream>

using namespace std;

Encyclopedia::Encyclopedia() {}

vector<PigeonInfo> Encyclopedia::getPigeons() const
{
    return pigeons;
}

void Encyclopedia::updateEncyclopedia(const string& name, float poopPerSecond, const string& description)
{
    for (const PigeonInfo& entry : pigeons)
        if (entry.name == name)
            return;
    pigeons.push_back({name, poopPerSecond, description});
}

void Encyclopedia::showPigeonInfo(const string& name) const  //test
{
    for (const PigeonInfo& entry : pigeons)
    {
        if (entry.name == name)
        {
            cout << "-->" << entry.name << "\n";
            cout << "Coins/sec: " << entry.poopPerSecond << "\n";
            cout << "Description: " << entry.description << "\n\n";
            return;
        }
    }
    cout << "No encyclopedia entry for " << name << "\n";
}

void Encyclopedia::showAll() const
{
    if (pigeons.empty())
    {
        cout << "The encyclopedia is empty. Discover pigeons by opening crates!\n";
        return;
    }
    cout << "\n         ENCYCLOPEDIA\n";
    for (const PigeonInfo& entry : pigeons)
    {
        std::cout << "-->" << entry.name << "\n";
        std::cout << "Coins/sec: " << entry.poopPerSecond << "\n";
        std::cout << "Description: " << entry.description << "\n\n";
    }
}