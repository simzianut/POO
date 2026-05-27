#include "Encyclopedia.h"
#include <iostream>
#include <utility>

using namespace std;

PigeonInfo::PigeonInfo() : poopPerSecond(0.0f) {}

PigeonInfo::PigeonInfo(string name, const float poopPerSecond, string description) :
    name(std::move(name)),
    poopPerSecond(poopPerSecond),
    description(std::move(description)) {}

PigeonInfo::PigeonInfo(const PigeonInfo& other) = default;

PigeonInfo& PigeonInfo::operator=(const PigeonInfo& other) = default;

PigeonInfo::~PigeonInfo() = default;

ostream& operator<<(ostream& out, const PigeonInfo& info)
{
    out << "-->" << info.name << "\n";
    out << "Coins/sec: " << info.poopPerSecond << "\n";
    out << "Description: " << info.description << "\n";
    return out;
}


Encyclopedia::Encyclopedia() = default;

vector<PigeonInfo> Encyclopedia::getPigeons() const
{
    return pigeons;
}

void Encyclopedia::updateEncyclopedia(const string& name, const float poopPerSecond, const string& description)
{
    for (const PigeonInfo& entry : pigeons)
        if (entry.name == name)
            return;
    pigeons.emplace_back(name, poopPerSecond, description);
}

void Encyclopedia::showPigeonInfo(const string& name) const  //test
{
    for (const PigeonInfo& entry : pigeons)
    {
        if (entry.name == name)
        {
            cout << entry << "\n";
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
        cout << entry << "\n";
}
