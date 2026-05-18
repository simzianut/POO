#ifndef OOP_POOP_H
#define OOP_POOP_H
#include <string>
using namespace std;

class Poop
{
protected:
    string color;
    int tier;
    int value;

public:
    Poop(string color, int tier, int value);
    int getPoopValue() const;
};
class clasicPoop : Poop
{
public:
    clasicPoop(const string& color, int tier, int value);
};
#endif //OOP_POOP_H