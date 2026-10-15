#pragma once

#include <SFML/Graphics.hpp>

/// Clickable, box-shaped visual for a closed pigeon crate.
class CrateSprite {
public:
    explicit CrateSprite(sf::Vector2f position);

    [[nodiscard]] bool contains(sf::Vector2f point) const;
    [[nodiscard]] sf::FloatRect getBounds() const;
    [[nodiscard]] sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;

private:
    sf::Vector2f       m_position;
    sf::RectangleShape m_shadow;
    sf::ConvexShape    m_topFace;
    sf::ConvexShape    m_sideFace;
    sf::RectangleShape m_frontFace;
    sf::RectangleShape m_frontBand;
    sf::RectangleShape m_latch;
};
