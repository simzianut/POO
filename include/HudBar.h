#pragma once

#include <array>
#include <string>
#include <SFML/Graphics.hpp>

class AssetManager;

enum class HudAction {
    None,
    Shop,
    Encyclopedia
};

/// Top bar (coins) + bottom bar (two action buttons) that always render above
/// the playfield.
class HudBar {
public:
    explicit HudBar(AssetManager& assets);

    /// Update hover state and coin counter.
    void update(sf::Vector2f mousePos, int coins);

    void draw(sf::RenderWindow& window) const;

    /// Returns the action associated with the clicked button, or HudAction::None.
    [[nodiscard]] HudAction handleClick(sf::Vector2f point) const;

private:
    struct Button {
        sf::RectangleShape shape;
        sf::Text           label;
        HudAction          action      = HudAction::None;
        sf::Color          normalColor;
        sf::Color          hoverColor;
        bool               hovered     = false;
    };

    void initButton(Button& btn, sf::FloatRect rect,
                    const std::string& label, HudAction action,
                    AssetManager& assets);

    AssetManager*          m_assets;
    sf::RectangleShape     m_topBar;
    sf::RectangleShape     m_bottomBar;
    sf::Text               m_coinText;
    sf::CircleShape        m_coinIcon;
    std::array<Button, 2>  m_buttons;
};
