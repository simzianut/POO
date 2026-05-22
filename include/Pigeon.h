#ifndef PIGEON_H
#define PIGEON_H

#include <string>
#include "Poop.h"

using namespace std;

class Pigeon {
protected:
    int tier;
    int poopRate;
    int level;

    [[nodiscard]] virtual bool hasNextEvolution() const;
    [[nodiscard]] virtual Pigeon* createNextEvolution() const = 0;

public:

    Pigeon(int tier, int poopRate, int level);
    Pigeon(const Pigeon& other);
    Pigeon& operator=(const Pigeon& other);
    virtual ~Pigeon();

    [[nodiscard]] virtual string getName() const = 0;

    [[nodiscard]] bool canMergeWith(const Pigeon& other) const;
    [[nodiscard]] Pigeon* merge(const Pigeon& other) const;
    [[nodiscard]] Poop* dropPoop() const;

    [[nodiscard]] int getTier() const;
    [[nodiscard]] int getPoopRate() const;
    [[nodiscard]] int getLevel() const;
};


class BabyPigeon : public Pigeon
{
public:
    BabyPigeon();

    [[nodiscard]] string getName() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};


class NormalPigeon : public Pigeon
{
public:
    NormalPigeon();

    [[nodiscard]] string getName() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};


class ChunkyPigeon : public Pigeon {
public:
    ChunkyPigeon();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};


class FatPigeon : public Pigeon {
public:
    FatPigeon();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};


class ObesePigeon : public Pigeon {
public:
    ObesePigeon();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};


class MutantPigeon : public Pigeon {
public:
    MutantPigeon();
    [[nodiscard]] string getName() const override;
    [[nodiscard]] bool hasNextEvolution() const override;
    [[nodiscard]] Pigeon* createNextEvolution() const override;
};
#endif
