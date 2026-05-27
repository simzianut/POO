#ifndef SHOP_H
#define SHOP_H

#include <string>
#include <vector>
#include "Berry.h"

class BoardManager;

struct PigeonOffer {
    int tier;
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

    [[nodiscard]] const PigeonOffer* findPigeonOffer(int pigeonTier) const;
    [[nodiscard]] PigeonOffer* findPigeonOffer(int pigeonTier);
    [[nodiscard]] const BerryOffer* findBerryOffer(BerryType berryType) const;

public:
    Shop();

    static void showCategories() ;
    void showPigeonCategory(const BoardManager& board) const;
    void showBerryCategory() const;

    [[nodiscard]] std::vector<PigeonOffer> getAvailablePigeonOffers(const BoardManager& board) const;
    [[nodiscard]] bool canBuyPigeon(const BoardManager& board, int pigeonTier) const;
    [[nodiscard]] int getPigeonPrice(int pigeonTier) const;
    [[nodiscard]] int getPigeonTimesBought(int pigeonTier) const;
    bool recordPigeonPurchase(int pigeonTier);

    [[nodiscard]] const std::vector<BerryOffer>& getBerryOffers() const;
    [[nodiscard]] bool canBuyBerry(BerryType berryType) const;
    [[nodiscard]] int getBerryPrice(BerryType berryType) const;
};

#endif
