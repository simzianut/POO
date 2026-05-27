#include "Pigeon.h"
#include "Poop.h"
#include <chrono>
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

bool operator==(const Pigeon& leftHandSide, const Pigeon& rightHandSide)
{
    return leftHandSide.tier == rightHandSide.tier && leftHandSide.hasNextEvolution() && rightHandSide.hasNextEvolution();
}

bool Pigeon::canMergeWith(const Pigeon& other) const
{
    if (dynamic_cast<const Pigeostrich*>(this) != nullptr ||
        dynamic_cast<const Pigeostrich*>(&other) != nullptr)
        return false;

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
