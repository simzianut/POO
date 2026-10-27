#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Encyclopedia.h"
#include "OverlayScrollBar.h"

class AssetManager;

/// Modal panel listing all discovered pigeon entries from the Encyclopedia.
class EncyclopediaOverlay {
public:
    explicit EncyclopediaOverlay(AssetManager& assets);

    void open();
    void close();
    [[nodiscard]] bool isOpen() const;

    /// Returns true if the click was consumed.
    bool handleClick(sf::Vector2f pos, const Encyclopedia& enc);
    bool handleMouseWheel(sf::Vector2f pos, float delta, const Encyclopedia& enc);
    bool handleMouseMoved(sf::Vector2f pos, const Encyclopedia& enc);
    void handleMouseReleased();

    void draw(sf::RenderWindow& window, const Encyclopedia& enc);

private:
    struct EncRow {
        sf::RectangleShape bg;
        sf::Sprite         icon;
        sf::Text           nameText;
        sf::Text           ppsText;
        sf::Text           descText;
    };

    void rebuildRows(const Encyclopedia& enc);

    bool               m_open  = false;
    AssetManager*      m_assets;

    sf::RectangleShape m_overlay;
    sf::RectangleShape m_panel;
    sf::RectangleShape m_closeBtn;
    sf::Text           m_title;
    sf::Text           m_closeLabel;

    std::vector<EncRow> m_rows;
    OverlayScrollBar    m_scrollBar;
    sf::FloatRect       m_contentViewport;
    bool                m_dirty = true;
};
