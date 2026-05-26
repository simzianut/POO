#ifndef SHOP_H
#define SHOP_H

#include <string>
#include <vector>
#include "Berry.h"

class BoardManager;

struct PigeonOffer {
    int level;
    std::string name;
    int currentPrice;
    int timesBought;
};

struct BerryOffer {
    BerryType type;
    std::string name;
    int price;
};

class Shop {
private:
    std::vector<PigeonOffer> pigeonOffers;
    std::vector<BerryOffer> berryOffers;

    [[nodiscard]] const PigeonOffer* findPigeonOffer(int pigeonLevel) const;
    [[nodiscard]] PigeonOffer* findPigeonOffer(int pigeonLevel);
    [[nodiscard]] const BerryOffer* findBerryOffer(BerryType berryType) const;

public:
    Shop();

    void showCategories() const;
    void showPigeonCategory(const BoardManager& board) const;
    void showBerryCategory() const;

    [[nodiscard]] std::vector<PigeonOffer> getAvailablePigeonOffers(const BoardManager& board) const;
    [[nodiscard]] bool canBuyPigeon(const BoardManager& board, int pigeonLevel) const;
    [[nodiscard]] int getPigeonPrice(int pigeonLevel) const;
    [[nodiscard]] int getPigeonTimesBought(int pigeonLevel) const;
    bool recordPigeonPurchase(int pigeonLevel);

    [[nodiscard]] std::vector<BerryOffer> getBerryOffers() const;
    [[nodiscard]] bool canBuyBerry(BerryType berryType) const;
    [[nodiscard]] int getBerryPrice(BerryType berryType) const;
};

#endif
