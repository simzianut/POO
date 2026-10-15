#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Berry.h"

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

    void draw(sf::RenderWindow& window, const BoardManager& board) const;

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
    };

    void rebuildRows(const BoardManager& board) const;

    bool               m_open  = false;
    AssetManager*      m_assets;

    sf::RectangleShape m_overlay;
    sf::RectangleShape m_panel;
    sf::RectangleShape m_closeBtn;
    sf::Text           m_title;
    sf::Text           m_closeLabel;
    mutable sf::Text   m_sectionPigeons;
    mutable sf::Text   m_sectionBerries;

    mutable std::vector<ShopRow> m_rows;
    mutable bool                 m_dirty = true;
};
