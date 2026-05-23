#include "Pigeon.h"
#include "Poop.h"
using namespace std;

Pigeon::Pigeon(int tier, int poopRate, int level) : tier(tier), poopRate(poopRate), level(level) {}
Pigeon::Pigeon(const Pigeon& other) : tier(other.tier), poopRate(other.poopRate), level(other.level) {}
Pigeon& Pigeon::operator=(const Pigeon& other)
{
    if (this != &other)
    {
        tier = other.tier;
        poopRate = other.poopRate;
        level = other.level;
    }
    return *this;
}
Pigeon::~Pigeon() = default;
bool Pigeon::canMergeWith(const Pigeon& other) const
{
    return tier == other.getTier() && hasNextEvolution();
}
bool Pigeon::hasNextEvolution() const
{
    return true;
}
Pigeon* Pigeon::merge(const Pigeon& other) const
{
    if (!canMergeWith(other))
        return nullptr;
    return createNextEvolution();
}
Poop* Pigeon::dropPoop() const
{
    return Poop::createByLevel(level);
}
Pigeon* Pigeon::createByLevel(int level)
{
    switch (level)
    {
        case 1:
            return new BabyPigeon();
        case 2:
            return new NormalPigeon();
        case 3:
            return new ChunkyPigeon();
        case 4:
            return new FatPigeon();
        case 5:
            return new ObesePigeon();
        case 6:
            return new MutantPigeon();
        default:
            return nullptr;
    }
}
int Pigeon::getTier() const { return tier; }
int Pigeon::getPoopRate() const { return poopRate; }
int Pigeon::getLevel() const { return level; }



BabyPigeon::BabyPigeon() : Pigeon(1, 1, 1) {}
string BabyPigeon::getName() const { return "Baby Pigeon"; }
Pigeon* BabyPigeon::createNextEvolution() const { return new NormalPigeon(); }

NormalPigeon::NormalPigeon() : Pigeon(1, 2, 2) {}
string NormalPigeon::getName() const { return "Normal Pigeon"; }
Pigeon* NormalPigeon::createNextEvolution() const { return new ChunkyPigeon(); }

ChunkyPigeon::ChunkyPigeon() : Pigeon(2, 5, 3) {}
std::string ChunkyPigeon::getName() const { return "Chunky Pigeon"; }
Pigeon* ChunkyPigeon::createNextEvolution() const { return new FatPigeon(); }

FatPigeon::FatPigeon() : Pigeon(2, 10, 4) {}
std::string FatPigeon::getName() const { return "Fat Pigeon"; }
Pigeon* FatPigeon::createNextEvolution() const { return new ObesePigeon(); }

ObesePigeon::ObesePigeon() : Pigeon(2, 30, 5) {}
std::string ObesePigeon::getName() const { return "Obese Pigeon"; }
Pigeon* ObesePigeon::createNextEvolution() const { return new MutantPigeon(); }

MutantPigeon::MutantPigeon() : Pigeon(3, 100, 6) {}
std::string MutantPigeon::getName() const { return "Mutant Pigeon"; }
Pigeon* MutantPigeon::createNextEvolution() const { return nullptr; }
bool MutantPigeon::hasNextEvolution() const
{
    return false;
}
