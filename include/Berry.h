#ifndef BERRY_H
#define BERRY_H

#include <string>
using namespace std;

enum class BerryType {
    None = 0,
    Red = 1,
    Yellow = 2,
    Purple = 3
};

class Berry {
protected:
    string color;
    BerryType type;

public:
    Berry(string color, BerryType type);
    virtual ~Berry() = default;

    [[nodiscard]] virtual string getName() const = 0;
    [[nodiscard]] string getColor() const;
    [[nodiscard]] BerryType getType() const;

    [[nodiscard]] static bool isValidType(BerryType type);
    [[nodiscard]] static BerryType typeFromInt(int value);
    [[nodiscard]] static string getNameByType(BerryType type);
    [[nodiscard]] static string getColorByType(BerryType type);
};

class RedBerry : public Berry {
public:
    RedBerry();
    [[nodiscard]] string getName() const override;
};

class YellowBerry : public Berry {
public:
    YellowBerry();
    [[nodiscard]] string getName() const override;
};

class PurpleBerry : public Berry {
public:
    PurpleBerry();
    [[nodiscard]] string getName() const override;
};

#endif
