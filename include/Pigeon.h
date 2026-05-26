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
    std::chrono::steady_clock::time_point lastPoopTime;
    BerryType activeBerryType;
    std::chrono::steady_clock::time_point berryEffectExpiration;

    [[nodiscard]] virtual bool hasNextEvolution() const;
    [[nodiscard]] virtual Pigeon* createNextEvolution() const = 0;

public:

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
    [[nodiscard]] bool hasNextEvolution() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};
#endif
