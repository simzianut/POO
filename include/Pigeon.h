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

    void addPoopsUntil(
        vector<Poop*>& poops,
        std::chrono::steady_clock::time_point endTime,
        std::chrono::steady_clock::duration interval
    );
    [[nodiscard]] virtual bool hasNextEvolution() const;
    [[nodiscard]] virtual Pigeon* createNextEvolution() const = 0;

public:
    friend bool operator==(const Pigeon& lhs, const Pigeon& rhs);

    // Constructor for pigeons available in the shop (tiers 1-13).
    Pigeon(int tier, int level, float weakChance, float strongChance, float pps, int basePrice);
    // Constructor for pigeons NOT available in the shop (tiers 14-16).
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


class BabyPigeon : public Pigeon
{
    string description;
public:
    BabyPigeon();

    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};


class NormalPigeon : public Pigeon
{
    string description;
public:
    NormalPigeon();

    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};


class ChunkyPigeon : public Pigeon {
    string description;
public:
    ChunkyPigeon();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};


class FatPigeon : public Pigeon {
    string description;
public:
    FatPigeon();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};


class ObesePigeon : public Pigeon {
    string description;
public:
    ObesePigeon();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};


class MutantPigeon : public Pigeon {
    string description;
public:
    MutantPigeon();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};

class PigeonWorm : public Pigeon {
    string description;
public:
    PigeonWorm();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};

class Spingeon : public Pigeon {
    string description;
public:
    Spingeon();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};

class Cheerlegeon : public Pigeon {
    string description;
public:
    Cheerlegeon();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};

class Chickenigeon : public Pigeon {
    string description;
public:
    Chickenigeon();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};

class Twingeon : public Pigeon {
    string description;
public:
    Twingeon();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};

class Pidgeknowledge : public Pigeon {
    string description;
public:
    Pidgeknowledge();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};

class Pidgeeyes : public Pigeon {
    string description;
public:
    Pidgeeyes();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};

// ── Final-tier pigeons: never sold in the shop ─────────────────────────────
// These use the no-price constructor overload and override isAvailableInShop()
// to return false, making their shop-exclusion explicit and polymorphic.

class Capturegeon : public Pigeon {
    string description;
public:
    Capturegeon();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
    [[nodiscard]] bool isAvailableInShop() const override;
};

class Bellybird : public Pigeon {
    string description;
public:
    Bellybird();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
    [[nodiscard]] bool isAvailableInShop() const override;
};

class Pigeostrich : public Pigeon {
    string description;
public:
    Pigeostrich();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] string getDescription() const override;
    [[nodiscard]] bool hasNextEvolution() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
    [[nodiscard]] bool isAvailableInShop() const override;
};
#endif
