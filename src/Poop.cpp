#include "Poop.h"

Poop::Poop(const int lvl, const int val) : level(lvl), value(val) {}

int Poop::collect() const { return value; }

Poop* Poop::createByLevel(const int level) {
    switch (level) {
        case 1:
            return new ClassicPoop();
        case 2:
            return new BronzePoop();
        case 3:
            return new SilverPoop();
        case 4:
            return new GoldenPoop();
        case 5:
            return new DiamondPoop();
        case 6:
            return new PlatinumPoop();
        default:
            return nullptr;
    }
}

int Poop::getLevel() const { return level; }
// cppcheck-suppress unusedFunction
int Poop::getValue() const { return value; }


ClassicPoop::ClassicPoop() : Poop(1, 1) {}
std::string ClassicPoop::getName() const { return "Classic Poop"; }

BronzePoop::BronzePoop() : Poop(2, 10) {}
std::string BronzePoop::getName() const { return "Bronze Poop"; }

SilverPoop::SilverPoop() : Poop(3, 100) {}
std::string SilverPoop::getName() const { return "Silver Poop"; }

GoldenPoop::GoldenPoop() : Poop(4, 1000) {}
std::string GoldenPoop::getName() const { return "Golden Poop"; }

DiamondPoop::DiamondPoop() : Poop(5, 10000) {}
std::string DiamondPoop::getName() const { return "Diamond Poop"; }

PlatinumPoop::PlatinumPoop() : Poop(6, 100000) {}
std::string PlatinumPoop::getName() const { return "Platinum Poop"; }
