#include "Poop.h"

#include <utility>

Poop::Poop(string color, const int tier, const int value) :
    color(std::move(color)), tier(tier), value(value)
{}

int Poop::getPoopValue() const {
    return value;
}

clasicPoop::clasicPoop(const string& color, int tier, int value) :
    Poop("brown", 1, 1)
{}
