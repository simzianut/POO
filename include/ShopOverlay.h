#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Berry.h"
#include "OverlayScrollBar.h"

class AssetManager;
class BoardManager;

/// Semi-transparent modal panel listing pigeon & berry offers.
/// Calls BoardManager::buyNewPigeon / buyNewBerry directly on purchase.
class ShopOverlay {
public:
    explicit ShopOverlay(AssetManager& assets);

    void open();
    void close();
    [[nodiscard]] bool isOpen() const;

    /// Returns true if the click was consumed by this overlay.
    bool handleClick(sf::Vector2f pos, BoardManager& board);
    bool handleMouseWheel(sf::Vector2f pos, float delta, const BoardManager& board);
    bool handleMouseMoved(sf::Vector2f pos, const BoardManager& board);
    void handleMouseReleased();

    void draw(sf::RenderWindow& window, const BoardManager& board);

private:
    struct ShopRow {
        sf::RectangleShape bg;
        sf::CircleShape    icon;
        sf::Text           nameText;
        sf::Text           priceText;
        sf::RectangleShape buyBtn;
        sf::Text           buyLabel;
        int                pigeonTier = 0;
        int                berryType  = 0;
        bool               isBerry    = false;
        bool               canAfford  = false;
        bool               isLocked   = false;
    };

    [[nodiscard]] bool needsRebuild(const BoardManager& board) const;
    void rebuildRows(const BoardManager& board);

    bool               m_open  = false;
    AssetManager*      m_assets;

    sf::RectangleShape m_overlay;
    sf::RectangleShape m_panel;
    sf::RectangleShape m_closeBtn;
    sf::Text           m_title;
    sf::Text           m_closeLabel;
    sf::Text           m_sectionPigeons;
    sf::Text           m_sectionBerries;

    std::vector<ShopRow> m_rows;
    OverlayScrollBar     m_scrollBar;
    sf::FloatRect        m_contentViewport;
    int                  m_cachedCoins = -1;
    int                  m_cachedBiggestLevel = -1;
    int                  m_cachedBiggestTier = -1;
    bool                 m_dirty = true;
};
