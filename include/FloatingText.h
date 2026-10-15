#pragma once

#include <string>
#include <SFML/Graphics.hpp>

/// A short-lived text label that rises upward and fades out.
/// Used for "+N coins", "EVOLVED!", "Can't merge!" feedback.
class FloatingText {
public:
    FloatingText(const sf::Font&      font,
                 const std::string&   str,
                 sf::Vector2f         startPos,
                 sf::Color            color    = sf::Color(255, 215, 0),
                 unsigned             charSize = 22);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    [[nodiscard]] bool isDead() const;

private:
    sf::Text      m_text;
    sf::Vector2f  m_velocity{ 0.f, -65.f };
    float         m_elapsed  = 0.f;
    float         m_lifetime = 1.5f;
};
