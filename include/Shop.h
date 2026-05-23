#ifndef SHOP_H
#define SHOP_H

class BoardManager;

class Shop {
public:
    void showCategories() const;
    void showPigeonCategory() const;
    void showBerryCategory() const;

    [[nodiscard]] bool canBuyPigeon(const BoardManager& board, int pigeonLevel) const;
    bool buyPigeon(BoardManager& board, int pigeonLevel) const;
};

#endif