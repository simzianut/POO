#ifndef ENCYCLOPEDIA_H
#define ENCYCLOPEDIA_H

#include <string>
#include <vector>
using namespace std;

struct PigeonInfo {
    string name;
    float  poopPerSecond;
    string description;
};

class Encyclopedia {
private:
    vector<PigeonInfo> pigeons;

public:
    Encyclopedia();

    [[nodiscard]] vector<PigeonInfo> getPigeons() const;
    void showPigeonInfo(const string& name) const;
    void showAll() const;

    void updateEncyclopedia(const string& name, float poopPerSecond, const string& description);
};

#endif
