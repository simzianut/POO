#include <algorithm>
#include <cstddef>
#include <memory>

#include <gtest/gtest.h>

#include "BoardManager.h"
#include "Encyclopedia.h"
#include "GameException.h"
#include "Pigeon.h"
#include "Shop.h"

struct BoardManagerTestAccess {
    static void reset(BoardManager& board)
    {
        for (Pigeon* pigeon : board.activePigeons)
            delete pigeon;
        board.activePigeons.clear();
        board.berryInventory.fill(0);
        board.encyclopedia = Encyclopedia{};
        board.shop = Shop{};
        board.berriesUnlocked = false;
        board.coins = 100000;
    }

    static void setBerryCount(BoardManager& board, BerryType type, int count)
    {
        board.berryInventory[static_cast<std::size_t>(type)] = count;
    }

    static void setCoins(BoardManager& board, int coins)
    {
        board.coins = coins;
    }

    static void removeLevelSixPlusPigeons(BoardManager& board)
    {
        auto& pigeons = board.activePigeons;
        for (auto it = pigeons.begin(); it != pigeons.end();) {
            if ((*it)->getLevel() >= Shop::BerryUnlockLevel) {
                delete *it;
                it = pigeons.erase(it);
            } else {
                ++it;
            }
        }
    }
};

namespace {
struct ResetBoardAfterTest {
    BoardManager& board;
    ~ResetBoardAfterTest() { BoardManagerTestAccess::reset(board); }
};
}

TEST(PigeonUnitTests, SameTierPigeonsMergeIntoNextTier)
{
    std::unique_ptr<Pigeon> left(Pigeon::createByTier(1));
    std::unique_ptr<Pigeon> right(Pigeon::createByTier(1));
    ASSERT_NE(left, nullptr);
    ASSERT_NE(right, nullptr);

    std::unique_ptr<Pigeon> evolved(left->merge(*right));
    ASSERT_NE(evolved, nullptr);
    EXPECT_EQ(evolved->getTier(), 2);
    EXPECT_EQ(evolved->getName(), "Normal Pigeon");
}

TEST(ShopUnitTests, BerryPricesIncreaseIndependentlyByThirtyPercent)
{
    Shop shop;
    EXPECT_EQ(shop.getBerryPrice(BerryType::Red), 500);
    EXPECT_TRUE(shop.recordBerryPurchase(BerryType::Red));
    EXPECT_EQ(shop.getBerryPrice(BerryType::Red), 650);
    EXPECT_EQ(shop.getBerryPrice(BerryType::Yellow), 500);
    EXPECT_TRUE(shop.recordBerryPurchase(BerryType::Red));
    EXPECT_EQ(shop.getBerryPrice(BerryType::Red), 845);
}

TEST(EncyclopediaUnitTests, StoresPigeonTierForArtworkSelection)
{
    Encyclopedia encyclopedia;
    encyclopedia.updateEncyclopedia("Capturegeon", 14, 12.f, "A pigeon with a net.");

    ASSERT_EQ(encyclopedia.getPigeons().size(), 1u);
    EXPECT_EQ(encyclopedia.getPigeons().front().tier, 14);
}

TEST(GameplayE2ETests, MainPlayerActionsAndFailuresBehaveAsExpected)
{
    BoardManager& board = BoardManager::getInstance();
    BoardManagerTestAccess::reset(board);
    ResetBoardAfterTest reset{board};

    board.spawnBabyPigeon(2);
    ASSERT_EQ(board.getTotalPigeonsAlive(), 2);
    EXPECT_NO_THROW(board.performMerge(0, 1));
    ASSERT_EQ(board.getTotalPigeonsAlive(), 1);
    EXPECT_EQ(board.getActivePigeons().front()->getTier(), 2);
    EXPECT_THROW(board.performMerge(0, 10), InvalidCommandException);

    Pigeon* cratePigeon = board.openCrate();
    ASSERT_NE(cratePigeon, nullptr);
    EXPECT_EQ(board.getTotalPigeonsAlive(), 2);

    board.addPigeon(Pigeon::createByTier(4));
    const int beforePigeonPurchase = board.getTotalPigeonsAlive();
    EXPECT_NO_THROW(board.buyNewPigeon(1));
    EXPECT_EQ(board.getTotalPigeonsAlive(), beforePigeonPurchase + 1);
    EXPECT_EQ(board.getPigeonPrice(1), 21);

    EXPECT_FALSE(board.canBuyBerry(BerryType::Red));
    EXPECT_EQ(board.getBerryOffers().front().price, 500);
    EXPECT_THROW(board.buyNewBerry(static_cast<int>(BerryType::Red)), GameException);
    EXPECT_EQ(board.getBerryOffers().front().price, 500);
    EXPECT_EQ(board.getBerryCount(BerryType::Red), 0);
    EXPECT_THROW(board.feedBerry(static_cast<int>(BerryType::Red), 0), GameException);

    board.addPigeon(Pigeon::createByTier(14)); // level 6 unlocks berries
    EXPECT_TRUE(board.canBuyBerry(BerryType::Red));
    const auto& entries = board.getEncyclopedia().getPigeons();
    const auto levelSixEntry = std::find_if(entries.begin(), entries.end(), [](const PigeonInfo& entry) {
        return entry.tier == 14;
    });
    ASSERT_NE(levelSixEntry, entries.end());
    EXPECT_NO_THROW(board.buyNewBerry(static_cast<int>(BerryType::Red)));
    EXPECT_EQ(board.getBerryCount(BerryType::Red), 1);
    EXPECT_EQ(board.getBerryOffers().front().price, 650);

    BoardManagerTestAccess::removeLevelSixPlusPigeons(board);
    EXPECT_TRUE(board.canBuyBerry(BerryType::Red)); // unlock remains for the session

    const int feedTarget = board.getPigeonIndex(board.getActivePigeons().front());
    EXPECT_NO_THROW(board.feedBerry(static_cast<int>(BerryType::Red), feedTarget));
    EXPECT_EQ(board.getBerryCount(BerryType::Red), 0);
    EXPECT_TRUE(board.getActivePigeons().front()->hasActiveBerryEffect());

    BoardManagerTestAccess::setBerryCount(board, BerryType::Yellow, 1);
    EXPECT_THROW(board.feedBerry(static_cast<int>(BerryType::Yellow), feedTarget),
                 BerryAlreadyInUseException);

    BoardManagerTestAccess::reset(board);
    BoardManagerTestAccess::setCoins(board, 0);
    board.addPigeon(Pigeon::createByTier(4));
    const int pigeonPriceBeforeFailure = board.getPigeonPrice(1);
    EXPECT_THROW(board.buyNewPigeon(1), NotEnoughCoinsException);
    EXPECT_EQ(board.getCoins(), 0);
    EXPECT_EQ(board.getPigeonPrice(1), pigeonPriceBeforeFailure);
}
