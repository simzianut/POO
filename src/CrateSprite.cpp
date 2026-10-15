#include "CrateSprite.h"

CrateSprite::CrateSprite(const sf::Vector2f position)
    : m_position(position)
{
    const sf::Color DARK_EDGE(72, 42, 25);

    m_shadow.setSize({72.f, 12.f});
    m_shadow.setPosition(position.x - 27.f, position.y + 32.f);
    m_shadow.setFillColor(sf::Color(0, 0, 0, 85));

    m_topFace.setPointCount(4);
    m_topFace.setPoint(0, {position.x - 32.f, position.y - 16.f});
    m_topFace.setPoint(1, {position.x - 20.f, position.y - 29.f});
    m_topFace.setPoint(2, {position.x + 44.f, position.y - 29.f});
    m_topFace.setPoint(3, {position.x + 32.f, position.y - 16.f});
    m_topFace.setFillColor(sf::Color(190, 132, 77));
    m_topFace.setOutlineColor(DARK_EDGE);
    m_topFace.setOutlineThickness(2.f);

    m_sideFace.setPointCount(4);
    m_sideFace.setPoint(0, {position.x + 32.f, position.y - 16.f});
    m_sideFace.setPoint(1, {position.x + 44.f, position.y - 29.f});
    m_sideFace.setPoint(2, {position.x + 44.f, position.y + 23.f});
    m_sideFace.setPoint(3, {position.x + 32.f, position.y + 36.f});
    m_sideFace.setFillColor(sf::Color(108, 62, 35));
    m_sideFace.setOutlineColor(DARK_EDGE);
    m_sideFace.setOutlineThickness(2.f);

    m_frontFace.setSize({64.f, 52.f});
    m_frontFace.setPosition(position.x - 32.f, position.y - 16.f);
    m_frontFace.setFillColor(sf::Color(151, 88, 44));
    m_frontFace.setOutlineColor(DARK_EDGE);
    m_frontFace.setOutlineThickness(2.f);

    m_frontBand.setSize({9.f, 52.f});
    m_frontBand.setPosition(position.x - 4.5f, position.y - 16.f);
    m_frontBand.setFillColor(sf::Color(218, 175, 91));
    m_frontBand.setOutlineColor(DARK_EDGE);
    m_frontBand.setOutlineThickness(1.f);

    m_latch.setSize({14.f, 13.f});
    m_latch.setPosition(position.x - 7.f, position.y + 1.f);
    m_latch.setFillColor(sf::Color(238, 207, 128));
    m_latch.setOutlineColor(DARK_EDGE);
    m_latch.setOutlineThickness(1.f);
}

bool CrateSprite::contains(const sf::Vector2f point) const
{
    return getBounds().contains(point);
}

sf::FloatRect CrateSprite::getBounds() const
{
    return {m_position.x - 32.f, m_position.y - 29.f, 76.f, 65.f};
}

sf::Vector2f CrateSprite::getPosition() const
{
    return m_position;
}

void CrateSprite::draw(sf::RenderWindow& window) const
{
    window.draw(m_shadow);
    window.draw(m_topFace);
    window.draw(m_sideFace);
    window.draw(m_frontFace);
    window.draw(m_frontBand);
    window.draw(m_latch);
}
