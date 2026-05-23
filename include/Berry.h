#ifndef BERRY_H
#define BERRY_H

#include <string>
using namespace std;

class Berry {
protected:
    string color;

public:
    explicit Berry(string color);
    virtual ~Berry() = default;

    [[nodiscard]] virtual string getName() const = 0;
    [[nodiscard]] string getColor() const;
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
