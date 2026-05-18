#ifndef OOP_PIGEON_H
#define OOP_PIGEON_H

#include "Poop.h"
#include <string>
using namespace std;

class Pigeon {
protected:
    string name;
    int tier;
    int poopRate;
    static int totalPigeonsAlive;

public:
    Pigeon(string name, int tier, int poopRate);
    Pigeon(const Pigeon& other);
    Pigeon& operator=(const Pigeon& other);
    virtual ~Pigeon();

    virtual int calculatePoopValue() const = 0;
    Poop* dropPoop() const;

};


#endif //OOP_PIGEON_H
