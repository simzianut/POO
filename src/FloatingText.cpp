#include "FloatingText.h"

#include <algorithm>

FloatingText::FloatingText(const sf::Font&    font,
                           const std::string& str,
                           sf::Vector2f       startPos,
                           sf::Color          color,
                           unsigned           charSize)
{
    m_text.setFont(font);
    m_text.setString(str);
    m_text.setCharacterSize(charSize);
    m_text.setFillColor(color);
    m_text.setStyle(sf::Text::Bold);
    m_text.setOutlineColor(sf::Color(0, 0, 0, 200));
    m_text.setOutlineThickness(2.f);

    // Centre the origin so the text floats from its visual centre
    sf::FloatRect b = m_text.getLocalBounds();
    m_text.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
    m_text.setPosition(startPos);
}

void FloatingText::update(float dt)
{
    m_elapsed += dt;
    m_text.move(m_velocity * dt);

    // Fade out during the second half of the lifetime
    float alpha = 1.f;
    const float fadeStart = m_lifetime * 0.45f;
    if (m_elapsed > fadeStart)
        alpha = std::max(0.f, 1.f - (m_elapsed - fadeStart) / (m_lifetime - fadeStart));

    auto applyAlpha = [&](sf::Color c, float a) {
        c.a = static_cast<sf::Uint8>(a * 255.f);
        return c;
    };
    m_text.setFillColor(applyAlpha(m_text.getFillColor(), alpha));
    m_text.setOutlineColor(applyAlpha(m_text.getOutlineColor(), alpha));
}

void FloatingText::draw(sf::RenderWindow& window) const
{
    window.draw(m_text);
}

bool FloatingText::isDead() const
{
    return m_elapsed >= m_lifetime;
}
