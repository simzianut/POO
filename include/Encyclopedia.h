#ifndef ENCYCLOPEDIA_H
#define ENCYCLOPEDIA_H

#include <string>
#include <vector>
using namespace std;

struct PigeonInfo {
    string name;
    int poopRate;
    string description;
};

class Encyclopedia {
private:
    vector<PigeonInfo> pigeons;

public:
    Encyclopedia();

    [[nodiscard]] vector<PigeonInfo> getPigeons() const;
    void showPigeonInfo(int level) const;
};

#endif
