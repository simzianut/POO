#ifndef POOP_H
#define POOP_H

#include <string>
using namespace std;

class Poop {
protected:
    int level;
    int value;

public:
    Poop(int lvl, int val);
    virtual ~Poop() = default;

    [[nodiscard]] virtual string getName() const = 0;

    [[nodiscard]] int collect() const;
    [[nodiscard]] static Poop* createByLevel(int level);

    [[nodiscard]] int getLevel() const;
    [[nodiscard]] int getValue() const;
};


class ClassicPoop : public Poop {
public:
    ClassicPoop();
    [[nodiscard]] string getName() const override;
};

using StandardPoop = ClassicPoop;

class BronzePoop : public Poop {
public:
    BronzePoop();
    [[nodiscard]] string getName() const override;
};

class SilverPoop : public Poop {
public:
    SilverPoop();
    [[nodiscard]] string getName() const override;
};

class GoldenPoop : public Poop {
public:
    GoldenPoop();
    [[nodiscard]] string getName() const override;
};

class PlatinumPoop : public Poop {
public:
    PlatinumPoop();
    [[nodiscard]] string getName() const override;
};

class DiamondPoop : public Poop {
public:
    DiamondPoop();
    [[nodiscard]] string getName() const override;
};

#endif
