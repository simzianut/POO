#include "Pigeon.h"
#include "Poop.h"
#include <chrono>
#include <cstdlib>
#include <vector>
using namespace std;


Pigeon::Pigeon(const int tier, const int level, const float weakChance, const float strongChance, const float pps, const int basePrice):
    tier(tier), level(level), weak_poop_chance(weakChance), strong_poop_chance(strongChance),
    poopPerSecond(pps), basePrice(basePrice), lastPoopTime(std::chrono::steady_clock::now()),
    activeBerryType(BerryType::None) {}


Pigeon::Pigeon(const int tier, const int level, const float weakChance, const float strongChance, const float pps):
    tier(tier), level(level), weak_poop_chance(weakChance), strong_poop_chance(strongChance),
    poopPerSecond(pps), basePrice(0), lastPoopTime(std::chrono::steady_clock::now()),
    activeBerryType(BerryType::None) {}

Pigeon::Pigeon(const Pigeon& other):
    tier(other.tier), level(other.level), weak_poop_chance(other.weak_poop_chance),
    strong_poop_chance(other.strong_poop_chance), poopPerSecond(other.poopPerSecond),
    basePrice(other.basePrice), lastPoopTime(other.lastPoopTime),
    activeBerryType(other.activeBerryType),
    berryEffectExpiration(other.berryEffectExpiration) {}

Pigeon& Pigeon::operator=(const Pigeon& other)
{
    if (this != &other)
    {
        tier = other.tier;
        level = other.level;
        weak_poop_chance = other.weak_poop_chance;
        strong_poop_chance = other.strong_poop_chance;
        poopPerSecond = other.poopPerSecond;
        basePrice = other.basePrice;
        lastPoopTime = other.lastPoopTime;
        activeBerryType = other.activeBerryType;
        berryEffectExpiration = other.berryEffectExpiration;
    }
    return *this;
}

Pigeon::~Pigeon() = default;

bool operator==(const Pigeon& lhs, const Pigeon& rhs)
{
    return lhs.tier == rhs.tier && lhs.hasNextEvolution() && rhs.hasNextEvolution();
}

bool Pigeon::canMergeWith(const Pigeon& other) const
{
    return *this == other;
}

bool Pigeon::hasNextEvolution() const
{
    return true;
}

Pigeon* Pigeon::merge(const Pigeon& other) const
{
    if (!canMergeWith(other))
        return nullptr;

    Pigeon* evolvedPigeon = createNextEvolution();
    if (evolvedPigeon == nullptr)
        return nullptr;

    if (hasActiveBerryEffect())
        evolvedPigeon->inheritBerryEffectFrom(*this);
    else if (other.hasActiveBerryEffect())
        evolvedPigeon->inheritBerryEffectFrom(other);

    return evolvedPigeon;
}


Poop* Pigeon::dropPoop() const
{
    float roll = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    int weakPoopLevel = level - 1;
    if (weakPoopLevel < 1)
        weakPoopLevel = 1;

    if (roll < weak_poop_chance)
        return Poop::createByLevel(weakPoopLevel);

    return Poop::createByLevel(level);
}

Poop* Pigeon::dropPoopIfReady()
{
    vector<Poop*> poops = dropPoopsIfReady();
    if (poops.empty())
        return nullptr;

    Poop* firstPoop = poops.front();
    for (size_t i = 1; i < poops.size(); i++)
        delete poops[i];

    return firstPoop;
}

void Pigeon::addPoopsUntil(
    vector<Poop*>& poops,
    const std::chrono::steady_clock::time_point endTime,
    const std::chrono::steady_clock::duration interval
)
{
    while (lastPoopTime + interval <= endTime)
    {
        lastPoopTime += interval;
        Poop* poop = dropPoop();
        if (poop != nullptr)
            poops.push_back(poop);
    }
}

vector<Poop*> Pigeon::dropPoopsIfReady()
{
    using Clock = std::chrono::steady_clock;

    vector<Poop*> poops;
    const Clock::time_point now = Clock::now();
    const Clock::duration normalInterval = std::chrono::seconds(2);
    const Clock::duration redInterval =
        std::chrono::duration_cast<Clock::duration>(std::chrono::duration<double>(0.25));

    if (activeBerryType == BerryType::Red)
    {
        Clock::time_point redEnd;

        if (now < berryEffectExpiration) {
            redEnd = now;
        } else {
            redEnd = berryEffectExpiration;
        }

       addPoopsUntil(poops, redEnd, redInterval);

        if (now >= berryEffectExpiration)
        {
            if (lastPoopTime < berryEffectExpiration)
                lastPoopTime = berryEffectExpiration;
            addPoopsUntil(poops, now, normalInterval);
        }
    }
    else
        addPoopsUntil(poops, now, normalInterval);
    return poops;
}

Pigeon* Pigeon::createByTier(const int tier)
{
    switch (tier)
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
        case 7:
            return new PigeonWorm();
        case 8:
            return new Spingeon();
        case 9:
            return new Cheerlegeon();
        case 10:
            return new Chickenigeon();
        case 11:
            return new Twingeon();
        case 12:
            return new Pidgeknowledge();
        case 13:
            return new Pidgeeyes();
        case 14:
            return new Capturegeon();
        case 15:
            return new Bellybird();
        case 16:
            return new Pigeostrich();
        default:
            return nullptr;
    }
}

Pigeon* Pigeon::createByLevel(const int level)
{
    return createByTier(level);
}

int Pigeon::getTier()const
{
    return tier;
}
int Pigeon::getLevel() const
{
    return level;
}
float Pigeon::getWeakPoopChance() const
{
    return weak_poop_chance;
}
float Pigeon::getStrongPoopChance() const
{
    return strong_poop_chance;
}
float Pigeon::getPoopPerSecond() const
{
    return poopPerSecond;
}
int Pigeon::getBasePrice() const
{
    return basePrice;
}
bool Pigeon::isAvailableInShop() const
{
    return true;
}

void Pigeon::applyBerryEffect(const BerryType type,const int durationSeconds)
{
    const auto now = std::chrono::steady_clock::now();
    activeBerryType = type;
    berryEffectExpiration = now + std::chrono::seconds(durationSeconds);

    if (type == BerryType::Red)
        lastPoopTime = now;
}

void Pigeon::clearBerryEffect()
{
    activeBerryType = BerryType::None;
    berryEffectExpiration = std::chrono::steady_clock::time_point();
}

void Pigeon::inheritBerryEffectFrom(const Pigeon& other)
{
    if (!other.hasActiveBerryEffect())
        return;

    activeBerryType = other.activeBerryType;
    berryEffectExpiration = other.berryEffectExpiration;

    if (activeBerryType == BerryType::Red)
        lastPoopTime = std::chrono::steady_clock::now();
}

bool Pigeon::hasBerryEffect() const
{
    return activeBerryType != BerryType::None;
}

bool Pigeon::hasActiveBerryEffect() const
{
    return hasBerryEffect() && std::chrono::steady_clock::now() < berryEffectExpiration;
}

bool Pigeon::hasActiveBerryEffect(const BerryType type) const
{
    return activeBerryType == type && hasActiveBerryEffect();
}

bool Pigeon::isBerryEffectExpired() const
{
    return hasBerryEffect() && std::chrono::steady_clock::now() >= berryEffectExpiration;
}

BerryType Pigeon::getActiveBerryType() const
{
    return activeBerryType;
}

int Pigeon::getRemainingBerrySeconds() const
{
    if (!hasBerryEffect())
        return 0;

    const auto now = std::chrono::steady_clock::now();
    if (now >= berryEffectExpiration)
        return 0;

    return static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(
        berryEffectExpiration - now).count());
}


BabyPigeon::BabyPigeon() : Pigeon(1, 1, 0.0f, 1.0f, 1.0f, 15),
    description("Cute baby") {}
string BabyPigeon::getName() const { return "Baby Pigeon"; }
string BabyPigeon::getDescription() const { return description; }
Pigeon* BabyPigeon::createNextEvolution() const { return new NormalPigeon(); }


NormalPigeon::NormalPigeon() : Pigeon(2, 2, 0.85f, 0.15f, 2.3f, 40),
    description("Your normal street pigeon") {}
string NormalPigeon::getName() const { return "Normal Pigeon"; }
string NormalPigeon::getDescription() const { return description; }
Pigeon* NormalPigeon::createNextEvolution() const { return new ChunkyPigeon(); }


ChunkyPigeon::ChunkyPigeon() : Pigeon(3, 2, 0.60f, 0.40f, 4.6f, 150),
    description("Ate too many peanuts") {}
string ChunkyPigeon::getName() const { return "Chunky Pigeon"; }
string ChunkyPigeon::getDescription() const { return description; }
Pigeon* ChunkyPigeon::createNextEvolution() const { return new FatPigeon(); }


FatPigeon::FatPigeon() : Pigeon(4, 2, 0.05f, 0.95f, 9.5f, 425),
    description("He just loves ice cream") {}
string FatPigeon::getName() const { return "Fat Pigeon"; }
string FatPigeon::getDescription() const { return description; }
Pigeon* FatPigeon::createNextEvolution() const { return new ObesePigeon(); }


ObesePigeon::ObesePigeon() : Pigeon(5, 3, 0.90f, 0.10f, 19.0f, 1000),
    description("Can barely walk, might need a doctor") {}
string ObesePigeon::getName() const { return "Obese Pigeon"; }
string ObesePigeon::getDescription() const { return description; }
Pigeon* ObesePigeon::createNextEvolution() const { return new MutantPigeon(); }


MutantPigeon::MutantPigeon() : Pigeon(6, 3, 0.65f, 0.35f, 41.5f, 2500),
    description("Something went very wrong at the lab") {}
string MutantPigeon::getName() const { return "Mutant Pigeon"; }
string MutantPigeon::getDescription() const { return description; }
Pigeon* MutantPigeon::createNextEvolution() const { return new PigeonWorm(); }


PigeonWorm::PigeonWorm() : Pigeon(7, 3, 0.15f, 0.85f, 86.5f, 3500),
    description("A pigeon that discovered worm mode") {}
string PigeonWorm::getName() const { return "Pigeonworm"; }
string PigeonWorm::getDescription() const { return description; }
Pigeon* PigeonWorm::createNextEvolution() const { return new Spingeon(); }


Spingeon::Spingeon() : Pigeon(8, 4, 0.90f, 0.10f, 190.0f, 5000),
    description("A spring-loaded pigeon with strange bounce") {}
string Spingeon::getName() const { return "Spingeon"; }
string Spingeon::getDescription() const { return description; }
Pigeon* Spingeon::createNextEvolution() const { return new Cheerlegeon(); }


Cheerlegeon::Cheerlegeon() : Pigeon(9, 4, 0.65f, 0.35f, 415.0f, 6900),
    description("Too enthusiastic for the nest") {}
string Cheerlegeon::getName() const { return "Cheerlegeon"; }
string Cheerlegeon::getDescription() const { return description; }
Pigeon* Cheerlegeon::createNextEvolution() const { return new Chickenigeon(); }


Chickenigeon::Chickenigeon() : Pigeon(10, 4, 0.15f, 0.85f, 865.0f, 9500),
    description("Somewhere between cluck and coo") {}
string Chickenigeon::getName() const { return "Chickenigeon"; }
string Chickenigeon::getDescription() const { return description; }
Pigeon* Chickenigeon::createNextEvolution() const { return new Twingeon(); }


Twingeon::Twingeon() : Pigeon(11, 5, 0.90f, 0.10f, 1900.0f, 13500),
    description("A suspiciously doubled pigeon") {}
string Twingeon::getName() const { return "Twingeon"; }
string Twingeon::getDescription() const { return description; }
Pigeon* Twingeon::createNextEvolution() const { return new Pidgeknowledge(); }


Pidgeknowledge::Pidgeknowledge() : Pigeon(12, 5, 0.65f, 0.35f, 4150.0f, 18000),
    description("Knows more than it should") {}
string Pidgeknowledge::getName() const { return "Pidgeknowledge"; }
string Pidgeknowledge::getDescription() const { return description; }
Pigeon* Pidgeknowledge::createNextEvolution() const { return new Pidgeeyes(); }


Pidgeeyes::Pidgeeyes() : Pigeon(13, 5, 0.15f, 0.85f, 8650.0f, 26000),
    description("Always watching the next merge") {}
string Pidgeeyes::getName() const { return "Pidgeyes"; }
string Pidgeeyes::getDescription() const { return description; }
Pigeon* Pidgeeyes::createNextEvolution() const { return new Capturegeon(); }




Capturegeon::Capturegeon() : Pigeon(14, 6, 0.90f, 0.10f, 19000.0f),
    description("A pigeon with collector instincts") {}
string Capturegeon::getName()        const { return "Capturegeon"; }
string Capturegeon::getDescription() const { return description; }
Pigeon* Capturegeon::createNextEvolution() const { return new Bellybird(); }
bool Capturegeon::isAvailableInShop() const { return false; }

Bellybird::Bellybird() : Pigeon(15, 6, 0.65f, 0.35f, 41500.0f),
    description("Mostly belly, technically bird") {}
string Bellybird::getName()        const { return "Bellybird"; }
string Bellybird::getDescription() const { return description; }
Pigeon* Bellybird::createNextEvolution() const { return new Pigeostrich(); }
bool Bellybird::isAvailableInShop() const { return false; }

Pigeostrich::Pigeostrich() : Pigeon(16, 6, 0.15f, 0.85f, 86500.0f),
    description("The tallest possible pigeon mistake") {}
string Pigeostrich::getName()        const { return "Pigeostrich"; }
string Pigeostrich::getDescription() const { return description; }
Pigeon* Pigeostrich::createNextEvolution() const { return nullptr; }
bool Pigeostrich::hasNextEvolution() const { return false; }
bool Pigeostrich::isAvailableInShop() const { return false; }
