#include "Pigeon.h"
#include "Poop.h"
#include <cstdlib>
#include <ctime>
using namespace std;


Pigeon::Pigeon(int tier, int level, float weakChance, float strongChance, float pps):
    tier(tier), level(level),weak_poop_chance(weakChance), strong_poop_chance(strongChance),
    poopPerSecond(pps), lastPoopTime(std::time(nullptr)) {}

Pigeon::Pigeon(const Pigeon& other):
    tier(other.tier),level(other.level), weak_poop_chance(other.weak_poop_chance),
    strong_poop_chance(other.strong_poop_chance),poopPerSecond(other.poopPerSecond),
    lastPoopTime(other.lastPoopTime) {}

Pigeon& Pigeon::operator=(const Pigeon& other)
{
    if (this != &other)
    {
        tier = other.tier;
        level = other.level;
        weak_poop_chance = other.weak_poop_chance;
        strong_poop_chance = other.strong_poop_chance;
        poopPerSecond = other.poopPerSecond;
        lastPoopTime = other.lastPoopTime;
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
    if (weak_poop_chance <= 0.0f)
        return Poop::createByLevel(level);

    float roll = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    if (roll < strong_poop_chance)
        return Poop::createByLevel(level);
    return Poop::createByLevel(level - 1);
}

Poop* Pigeon::dropPoopIfReady()
{
    std::time_t now = std::time(nullptr);
    if (std::difftime(now, lastPoopTime) >= 2.0)
    {
        lastPoopTime = now;
        return dropPoop();
    }
    return nullptr;
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
int Pigeon::getLevel() const { return level; }
float Pigeon::getWeakPoopChance() const { return weak_poop_chance; }
float Pigeon::getStrongPoopChance() const { return strong_poop_chance; }
float Pigeon::getPoopPerSecond() const { return poopPerSecond; }


BabyPigeon::BabyPigeon() : Pigeon(1, 1, 0.0f, 1.0f, 1.0f),
    description("Cute baby") {}
string BabyPigeon::getName() const { return "Baby Pigeon"; }
string BabyPigeon::getDescription() const { return description; }
Pigeon* BabyPigeon::createNextEvolution() const { return new NormalPigeon(); }


NormalPigeon::NormalPigeon() : Pigeon(2, 2, 0.85f, 0.15f, 2.3f),
    description("Your normal street pigeon") {}
string NormalPigeon::getName() const { return "Normal Pigeon"; }
string NormalPigeon::getDescription() const { return description; }
Pigeon* NormalPigeon::createNextEvolution() const { return new ChunkyPigeon(); }


ChunkyPigeon::ChunkyPigeon() : Pigeon(3, 3, 0.60f, 0.40f, 4.6f),
    description("Ate too many peanuts") {}
string ChunkyPigeon::getName() const { return "Chunky Pigeon"; }
string ChunkyPigeon::getDescription() const { return description; }
Pigeon* ChunkyPigeon::createNextEvolution() const { return new FatPigeon(); }


FatPigeon::FatPigeon() : Pigeon(4, 4, 0.05f, 0.95f, 9.5f),
    description("He just loves ice cream") {}
string FatPigeon::getName() const { return "Fat Pigeon"; }
string FatPigeon::getDescription() const { return description; }
Pigeon* FatPigeon::createNextEvolution() const { return new ObesePigeon(); }


ObesePigeon::ObesePigeon() : Pigeon(5, 5, 0.90f, 0.10f, 19.0f),
    description("Can barely walk, might need a doctor") {}
string ObesePigeon::getName() const { return "Obese Pigeon"; }
string ObesePigeon::getDescription() const { return description; }
Pigeon* ObesePigeon::createNextEvolution() const { return new MutantPigeon(); }


MutantPigeon::MutantPigeon() : Pigeon(6, 6, 0.65f, 0.35f, 41.5f),
    description("Something went very wrong at the lab") {}
string MutantPigeon::getName() const { return "Mutant Pigeon"; }
string MutantPigeon::getDescription() const { return description; }
Pigeon* MutantPigeon::createNextEvolution() const { return nullptr; }
bool MutantPigeon::hasNextEvolution() const { return false; }
