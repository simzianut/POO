#pragma once

#include <SFML/Graphics.hpp>
#include "Berry.h"

class AssetManager;

/// Visual representation of a berry in the berry tray.
/// Click to select, then click a pigeon to feed it.
class BerrySprite {
public:
    BerrySprite(BerryType type, AssetManager& assets, sf::Vector2f position);

    void draw(sf::RenderWindow& window) const;

    [[nodiscard]] bool         contains(sf::Vector2f point) const;
    [[nodiscard]] sf::Vector2f getPosition()                const;
    [[nodiscard]] BerryType    getType()                    const;

    void setSelected(bool selected);
    [[nodiscard]] bool isSelected() const;

private:
    static constexpr float RADIUS = 26.f;

    BerryType       m_type;
    sf::CircleShape m_glow;     ///< selection ring (only drawn when selected)
    sf::CircleShape m_circle;
    sf::Text        m_label;    ///< first letter of berry name
    bool            m_selected = false;
};
