#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Encyclopedia.h"

class AssetManager;

/// Modal panel listing all discovered pigeon entries from the Encyclopedia.
class EncyclopediaOverlay {
public:
    explicit EncyclopediaOverlay(AssetManager& assets);

    void open();
    void close();
    [[nodiscard]] bool isOpen() const;

    /// Returns true if the click was consumed.
    bool handleClick(sf::Vector2f pos);

    void draw(sf::RenderWindow& window, const Encyclopedia& enc) const;

private:
    struct EncRow {
        sf::RectangleShape bg;
        sf::Text           nameText;
        sf::Text           ppsText;
        sf::Text           descText;
    };

    void rebuildRows(const Encyclopedia& enc) const;

    bool               m_open  = false;
    AssetManager*      m_assets;

    sf::RectangleShape m_overlay;
    sf::RectangleShape m_panel;
    sf::RectangleShape m_closeBtn;
    sf::Text           m_title;
    sf::Text           m_closeLabel;

    mutable std::vector<EncRow> m_rows;
    mutable bool                m_dirty = true;
};
