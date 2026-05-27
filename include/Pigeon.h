#ifndef PIGEON_H
#define PIGEON_H

#include <chrono>
#include <string>
#include <vector>
#include "Berry.h"
#include "Poop.h"

using namespace std;

class Pigeon {
protected:
    int tier;
    int level;
    float weak_poop_chance;
    float strong_poop_chance;
    float poopPerSecond;
    int basePrice;
    std::chrono::steady_clock::time_point lastPoopTime;
    BerryType activeBerryType;
    std::chrono::steady_clock::time_point berryEffectExpiration;

    void addPoopsUntil( vector<Poop*>& poops,
        std::chrono::steady_clock::time_point endTime, std::chrono::steady_clock::duration interval);
    [[nodiscard]] virtual bool hasNextEvolution() const;
    [[nodiscard]] virtual Pigeon* createNextEvolution() const = 0;

public:
    friend bool operator==(const Pigeon& leftHandSide, const Pigeon& rightHandSide);

    Pigeon(int tier, int level, float weakChance, float strongChance, float pps, int basePrice);
    Pigeon(int tier, int level, float weakChance, float strongChance, float pps);
    Pigeon(const Pigeon& other);
    Pigeon& operator=(const Pigeon& other);
    virtual ~Pigeon();

    [[nodiscard]] virtual string getName() const = 0;
    [[nodiscard]] virtual string getDescription() const = 0;

    [[nodiscard]] bool canMergeWith(const Pigeon& other) const;
    [[nodiscard]] Pigeon* merge(const Pigeon& other) const;
    [[nodiscard]] Poop* dropPoop() const;
    [[nodiscard]] Poop* dropPoopIfReady();
    [[nodiscard]] vector<Poop*> dropPoopsIfReady();
    [[nodiscard]] static Pigeon* createByTier(int tier);
    [[nodiscard]] static Pigeon* createByLevel(int level);

    void applyBerryEffect(BerryType type, int durationSeconds);
    void clearBerryEffect();
    void inheritBerryEffectFrom(const Pigeon& other);
    [[nodiscard]] bool hasBerryEffect() const;
    [[nodiscard]] bool hasActiveBerryEffect() const;
    [[nodiscard]] bool hasActiveBerryEffect(BerryType type) const;
    [[nodiscard]] bool isBerryEffectExpired() const;
    [[nodiscard]] BerryType getActiveBerryType() const;
    [[nodiscard]] int getRemainingBerrySeconds() const;

    [[nodiscard]] int getTier() const;
    [[nodiscard]] int getLevel() const;
    [[nodiscard]] float getWeakPoopChance() const;
    [[nodiscard]] float getStrongPoopChance() const;
    [[nodiscard]] float getPoopPerSecond() const;
    [[nodiscard]] virtual int getBasePrice() const;
    [[nodiscard]] virtual bool isAvailableInShop() const;
};



template <typename Traits>
class ShopPigeon final : public Pigeon {
protected:
    [[nodiscard]] Pigeon* createNextEvolution() const override
    {
        return Pigeon::createByTier(Traits::tier + 1);
    }

public:
    ShopPigeon() :
        Pigeon(
            Traits::tier,
            Traits::level,
            Traits::weakPoopChance,
            Traits::strongPoopChance,
            Traits::poopPerSecond,
            Traits::basePrice
        ) {}

    ShopPigeon(const ShopPigeon& other) : Pigeon(other) {}

    ShopPigeon& operator=(const ShopPigeon& other)
    {
        Pigeon::operator=(other);
        return *this;
    }

    ~ShopPigeon() override = default;

    [[nodiscard]] string getName() const override
    {
        return Traits::name;
    }

    [[nodiscard]] string getDescription() const override
    {
        return Traits::description;
    }
};


template <typename Traits>
class TerminalPigeon final : public Pigeon {
protected:
    [[nodiscard]] bool hasNextEvolution() const override
    {
        return false;
    }

    [[nodiscard]] Pigeon* createNextEvolution() const override
    {
        return nullptr;
    }

public:
    TerminalPigeon() :
        Pigeon(
            Traits::tier,
            Traits::level,
            Traits::weakPoopChance,
            Traits::strongPoopChance,
            Traits::poopPerSecond,
            Traits::basePrice
        ) {}

    TerminalPigeon(const TerminalPigeon& other) : Pigeon(other) {}

    TerminalPigeon& operator=(const TerminalPigeon& other)
    {
        Pigeon::operator=(other);
        return *this;
    }

    ~TerminalPigeon() override = default;

    [[nodiscard]] string getName() const override
    {
        return Traits::name;
    }

    [[nodiscard]] string getDescription() const override
    {
        return Traits::description;
    }

};

struct BabyPigeonTraits {
    static constexpr int tier = 1;
    static constexpr int level = 1;
    static constexpr float weakPoopChance = 0.0f;
    static constexpr float strongPoopChance = 1.0f;
    static constexpr float poopPerSecond = 1.0f;
    static constexpr int basePrice = 15;
    static constexpr const char* name = "Baby Pigeon";
    static constexpr const char* description = "Cute baby";
};

struct NormalPigeonTraits {
    static constexpr int tier = 2;
    static constexpr int level = 2;
    static constexpr float weakPoopChance = 0.85f;
    static constexpr float strongPoopChance = 0.15f;
    static constexpr float poopPerSecond = 2.3f;
    static constexpr int basePrice = 40;
    static constexpr const char* name = "Normal Pigeon";
    static constexpr const char* description = "Your normal street pigeon";
};

struct ChunkyPigeonTraits {
    static constexpr int tier = 3;
    static constexpr int level = 2;
    static constexpr float weakPoopChance = 0.60f;
    static constexpr float strongPoopChance = 0.40f;
    static constexpr float poopPerSecond = 4.6f;
    static constexpr int basePrice = 150;
    static constexpr const char* name = "Chunky Pigeon";
    static constexpr const char* description = "Ate too many peanuts";
};

struct FatPigeonTraits {
    static constexpr int tier = 4;
    static constexpr int level = 2;
    static constexpr float weakPoopChance = 0.05f;
    static constexpr float strongPoopChance = 0.95f;
    static constexpr float poopPerSecond = 9.5f;
    static constexpr int basePrice = 425;
    static constexpr const char* name = "Fat Pigeon";
    static constexpr const char* description = "He just loves ice cream";
};

struct ObesePigeonTraits {
    static constexpr int tier = 5;
    static constexpr int level = 3;
    static constexpr float weakPoopChance = 0.90f;
    static constexpr float strongPoopChance = 0.10f;
    static constexpr float poopPerSecond = 19.0f;
    static constexpr int basePrice = 1000;
    static constexpr const char* name = "Obese Pigeon";
    static constexpr const char* description = "Can barely walk, might need a doctor";
};

struct MutantPigeonTraits {
    static constexpr int tier = 6;
    static constexpr int level = 3;
    static constexpr float weakPoopChance = 0.65f;
    static constexpr float strongPoopChance = 0.35f;
    static constexpr float poopPerSecond = 41.5f;
    static constexpr int basePrice = 2500;
    static constexpr const char* name = "Mutant Pigeon";
    static constexpr const char* description = "Something went very wrong at the lab";
};

struct PigeonWormTraits {
    static constexpr int tier = 7;
    static constexpr int level = 3;
    static constexpr float weakPoopChance = 0.15f;
    static constexpr float strongPoopChance = 0.85f;
    static constexpr float poopPerSecond = 86.5f;
    static constexpr int basePrice = 3500;
    static constexpr const char* name = "Pigeonworm";
    static constexpr const char* description = "A pigeon that discovered worm mode";
};

struct SpingeonTraits {
    static constexpr int tier = 8;
    static constexpr int level = 4;
    static constexpr float weakPoopChance = 0.90f;
    static constexpr float strongPoopChance = 0.10f;
    static constexpr float poopPerSecond = 190.0f;
    static constexpr int basePrice = 4900;
    static constexpr const char* name = "Spingeon";
    static constexpr const char* description = "A spring-loaded pigeon with strange bounce";
};

struct CheerlegeonTraits {
    static constexpr int tier = 9;
    static constexpr int level = 4;
    static constexpr float weakPoopChance = 0.65f;
    static constexpr float strongPoopChance = 0.35f;
    static constexpr float poopPerSecond = 415.0f;
    static constexpr int basePrice = 6860;
    static constexpr const char* name = "Cheerlegeon";
    static constexpr const char* description = "Too enthusiastic for the nest";
};

struct ChickenigeonTraits {
    static constexpr int tier = 10;
    static constexpr int level = 4;
    static constexpr float weakPoopChance = 0.15f;
    static constexpr float strongPoopChance = 0.85f;
    static constexpr float poopPerSecond = 865.0f;
    static constexpr int basePrice = 9604;
    static constexpr const char* name = "Chickenigeon";
    static constexpr const char* description = "Somewhere between cluck and coo";
};

struct TwingeonTraits {
    static constexpr int tier = 11;
    static constexpr int level = 5;
    static constexpr float weakPoopChance = 0.90f;
    static constexpr float strongPoopChance = 0.10f;
    static constexpr float poopPerSecond = 1900.0f;
    static constexpr int basePrice = 13445;
    static constexpr const char* name = "Twingeon";
    static constexpr const char* description = "A suspiciously doubled pigeon";
};

struct PidgeknowledgeTraits {
    static constexpr int tier = 12;
    static constexpr int level = 5;
    static constexpr float weakPoopChance = 0.65f;
    static constexpr float strongPoopChance = 0.35f;
    static constexpr float poopPerSecond = 4150.0f;
    static constexpr int basePrice = 18823;
    static constexpr const char* name = "Pidgeknowledge";
    static constexpr const char* description = "Knows more than it should";
};

struct PidgeeyesTraits {
    static constexpr int tier = 13;
    static constexpr int level = 5;
    static constexpr float weakPoopChance = 0.15f;
    static constexpr float strongPoopChance = 0.85f;
    static constexpr float poopPerSecond = 8650.0f;
    static constexpr int basePrice = 26352;
    static constexpr const char* name = "Pidgeyes";
    static constexpr const char* description = "Always watching the next merge";
};

struct CapturegeonTraits {
    static constexpr int tier = 14;
    static constexpr int level = 6;
    static constexpr float weakPoopChance = 0.90f;
    static constexpr float strongPoopChance = 0.10f;
    static constexpr float poopPerSecond = 19000.0f;
    static constexpr int basePrice = 36892;
    static constexpr const char* name = "Capturegeon";
    static constexpr const char* description = "A pigeon with collector instincts";
};

struct BellybirdTraits {
    static constexpr int tier = 15;
    static constexpr int level = 6;
    static constexpr float weakPoopChance = 0.65f;
    static constexpr float strongPoopChance = 0.35f;
    static constexpr float poopPerSecond = 41500.0f;
    static constexpr int basePrice = 51648;
    static constexpr const char* name = "Bellybird";
    static constexpr const char* description = "Mostly belly, technically bird";
};

struct PigeostrichTraits {
    static constexpr int tier = 16;
    static constexpr int level = 6;
    static constexpr float weakPoopChance = 0.15f;
    static constexpr float strongPoopChance = 0.85f;
    static constexpr float poopPerSecond = 86500.0f;
    static constexpr int basePrice = 72307;
    static constexpr const char* name = "Pigeostrich";
    static constexpr const char* description = "The tallest possible pigeon mistake";
};

using BabyPigeon = ShopPigeon<BabyPigeonTraits>;
using NormalPigeon = ShopPigeon<NormalPigeonTraits>;
using ChunkyPigeon = ShopPigeon<ChunkyPigeonTraits>;
using FatPigeon = ShopPigeon<FatPigeonTraits>;
using ObesePigeon = ShopPigeon<ObesePigeonTraits>;
using MutantPigeon = ShopPigeon<MutantPigeonTraits>;
using PigeonWorm = ShopPigeon<PigeonWormTraits>;
using Spingeon = ShopPigeon<SpingeonTraits>;
using Cheerlegeon = ShopPigeon<CheerlegeonTraits>;
using Chickenigeon = ShopPigeon<ChickenigeonTraits>;
using Twingeon = ShopPigeon<TwingeonTraits>;
using Pidgeknowledge = ShopPigeon<PidgeknowledgeTraits>;
using Pidgeeyes = ShopPigeon<PidgeeyesTraits>;

using Capturegeon = ShopPigeon<CapturegeonTraits>;
using Bellybird = ShopPigeon<BellybirdTraits>;
using Pigeostrich = TerminalPigeon<PigeostrichTraits>;

#endif
