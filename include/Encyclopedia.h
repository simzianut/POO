#ifndef ENCYCLOPEDIA_H
#define ENCYCLOPEDIA_H

#include <iosfwd>
#include <string>
#include <vector>
using namespace std;

struct PigeonInfo
{
    string name;
    float  poopPerSecond;
    string description;

    PigeonInfo();
    PigeonInfo(string name, float poopPerSecond, string description);
    PigeonInfo(const PigeonInfo& other);
    PigeonInfo& operator=(const PigeonInfo& other);
    ~PigeonInfo();


    friend ostream& operator<<(ostream& out, const PigeonInfo& info);
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
