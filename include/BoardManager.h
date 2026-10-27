#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H

#include <array>
#include <iosfwd>
#include <string>
#include <vector>
#include "Pigeon.h"
#include "Encyclopedia.h"
#include "Shop.h"

using namespace std;

class BoardManager {
private:
    vector<Pigeon*> activePigeons;
    array<int, 4> berryInventory;
    Encyclopedia encyclopedia;
    Shop shop;
    bool berriesUnlocked;
    int coins;
    int berryEffectDurationSeconds;
    int lastInputCommand;
    string expectedInputName;

    [[nodiscard]] bool isValidPigeonIndex(int index) const;
    [[nodiscard]] int getBerryInventoryCount(BerryType berryType) const;
    [[nodiscard]] bool hasDiscoveredLargestPigeon() const;
    [[nodiscard]] bool readIntInput(const string& inputName, int& value);
    void sortPigeonsByTier();

    BoardManager();


public:
    friend istream& operator>>(istream& in, BoardManager& board);

    ~BoardManager();
    static BoardManager& getInstance();

    BoardManager(const BoardManager&) = delete;
    BoardManager &operator=(BoardManager) = delete;

    /// Opens a crate, adds its pigeon to the board, and returns that pigeon.
    Pigeon* openCrate();
    void addPigeon(Pigeon* pigeon);
    void spawnBabyPigeon(int count=1);
    void spawnMutantPigeon();
    void performMerge(int index1, int index2);
    void performMerge(int index);
    [[nodiscard]] int getTotalPigeonsAlive() const;
    [[nodiscard]] int getBiggestPigeonLevel() const;
    [[nodiscard]] int getBiggestPigeonTier() const;
    [[nodiscard]] int getCoins() const;
    [[nodiscard]] bool canBuyBerry(BerryType berryType) const;
    [[nodiscard]] bool hasAnyActiveBerryEffect() const;
    /// Returns how many berries of the given type are in inventory.
    [[nodiscard]] int getBerryCount(BerryType berryType) const;

    // ── GUI read-only accessors ─────────────────────────────────────────────
    /// Returns the index of p in activePigeons, or -1 if not found.
    [[nodiscard]] int getPigeonIndex(const Pigeon* p) const;
    /// Direct read access to the active pigeon list.
    [[nodiscard]] const std::vector<Pigeon*>& getActivePigeons() const;
    /// Delegates to Shop::getAvailablePigeonOffers.
    [[nodiscard]] std::vector<PigeonOffer> getAvailablePigeonOffers() const;
    /// Delegates to Shop::getBerryOffers.
    [[nodiscard]] const std::vector<BerryOffer>& getBerryOffers() const;
    /// Delegates to Shop::getPigeonPrice.
    [[nodiscard]] int getPigeonPrice(int pigeonTier) const;
    /// Read access to the Encyclopedia for the GUI Encyclopedia overlay.
    [[nodiscard]] const Encyclopedia& getEncyclopedia() const;
    void printBoard();
    void printBerryInventory() const;

    void update();

    void showEncyclopedia() const;
    void showShop();
    void showFeedBerryMenu();
    void buyNewPigeon(int desiredPigeonTier);
    void buyNewBerry(int desiredBerryType);
    void feedBerry(int desiredBerryType, int pigeonIndex);
};

#endif
