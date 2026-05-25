#ifndef SHOP_H
#define SHOP_H
#include <string>
#include <vector>

class BoardManager;

using namespace std;

struct PigeonOffer {
    int level;
    string name;
    int currentPrice;
    int timesBought;
};

class Shop {
private:
    vector<PigeonOffer> pigeonOffers;

    [[nodiscard]] const PigeonOffer* findPigeonOffer(int pigeonLevel) const;
    [[nodiscard]] PigeonOffer* findPigeonOffer(int pigeonLevel);

public:
    Shop();

    static void showCategories() ;
    void showPigeonCategory(const BoardManager& board) const;
    static void showBerryCategory() ;

    [[nodiscard]] std::vector<PigeonOffer> getAvailablePigeonOffers(const BoardManager& board) const;
    [[nodiscard]] bool canBuyPigeon(const BoardManager& board, int pigeonLevel) const;
    [[nodiscard]] int getPigeonPrice(int pigeonLevel) const;
    [[nodiscard]] int getPigeonTimesBought(int pigeonLevel) const;
    bool recordPigeonPurchase(int pigeonLevel);
};

#endif
