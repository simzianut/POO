#include "Berry.h"

Berry::Berry(string color) : color(std::move(color)) {}

string Berry::getColor() const { return color; }

RedBerry::RedBerry() : Berry("red") {}
string RedBerry::getName() const { return "Red Berry"; }

YellowBerry::YellowBerry() : Berry("yellow") {}
string YellowBerry::getName() const { return "Yellow Berry"; }

PurpleBerry::PurpleBerry() : Berry("purple") {}
string PurpleBerry::getName() const { return "Purple Berry"; }
