#include "Berry.h"

Berry::Berry(string color, BerryType type) : color(std::move(color)), type(type) {}

string Berry::getColor() const { return color; }
BerryType Berry::getType() const { return type; }

bool Berry::isValidType(BerryType type)
{
    return type == BerryType::Red ||
           type == BerryType::Yellow ||
           type == BerryType::Purple;
}

BerryType Berry::typeFromInt(int value)
{
    BerryType type = static_cast<BerryType>(value);
    if (!isValidType(type))
        return BerryType::None;

    return type;
}

string Berry::getNameByType(BerryType type)
{
    switch (type)
    {
        case BerryType::Red:
            return "Red Berry";
        case BerryType::Yellow:
            return "Yellow Berry";
        case BerryType::Purple:
            return "Purple Berry";
        default:
            return "No Berry";
    }
}

string Berry::getColorByType(BerryType type)
{
    switch (type)
    {
        case BerryType::Red:
            return "red";
        case BerryType::Yellow:
            return "yellow";
        case BerryType::Purple:
            return "purple";
        default:
            return "none";
    }
}

RedBerry::RedBerry() : Berry("red", BerryType::Red) {}
string RedBerry::getName() const { return "Red Berry"; }

YellowBerry::YellowBerry() : Berry("yellow", BerryType::Yellow) {}
string YellowBerry::getName() const { return "Yellow Berry"; }

PurpleBerry::PurpleBerry() : Berry("purple", BerryType::Purple) {}
string PurpleBerry::getName() const { return "Purple Berry"; }
