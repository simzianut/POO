#include "BerrySprite.h"
#include "AssetManager.h"

#include <string>

// ---------------------------------------------------------------------------
// Colour helper
// ---------------------------------------------------------------------------
static sf::Color berryFill(BerryType type)
{
    switch (type) {
        case BerryType::Red:    return { 215,  50,  50 };
        case BerryType::Yellow: return { 255, 210,   0 };
        case BerryType::Purple: return { 155,   0, 210 };
        default:                return { 150, 150, 150 };
    }
}

static sf::Color berryOutline(BerryType type)
{
    switch (type) {
        case BerryType::Red:    return { 255, 120, 120 };
        case BerryType::Yellow: return { 255, 240, 100 };
        case BerryType::Purple: return { 200, 100, 255 };
        default:                return { 200, 200, 200 };
    }
}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
BerrySprite::BerrySprite(BerryType type, AssetManager& assets, sf::Vector2f position)
    : m_type(type)
{
    sf::Color fill    = berryFill(type);
    sf::Color outline = berryOutline(type);

    // Glow ring (selection indicator)
    m_glow.setRadius(RADIUS + 8.f);
    m_glow.setOrigin(RADIUS + 8.f, RADIUS + 8.f);
    m_glow.setFillColor(sf::Color(outline.r, outline.g, outline.b, 55));
    m_glow.setOutlineColor(sf::Color(outline.r, outline.g, outline.b, 220));
    m_glow.setOutlineThickness(3.f);
    m_glow.setPosition(position);

    // Main berry circle
    m_circle.setRadius(RADIUS);
    m_circle.setOrigin(RADIUS, RADIUS);
    m_circle.setFillColor(fill);
    m_circle.setOutlineColor(outline);
    m_circle.setOutlineThickness(2.5f);
    m_circle.setPosition(position);

    // First-letter label (R / Y / P)
    std::string name = Berry::getNameByType(type);
    m_label.setFont(assets.getFont("main"));
    m_label.setString(name.empty() ? "?" : std::string(1, name[0]));
    m_label.setCharacterSize(20);
    m_label.setFillColor(sf::Color::White);
    m_label.setStyle(sf::Text::Bold);
    m_label.setOutlineColor(sf::Color(0, 0, 0, 180));
    m_label.setOutlineThickness(1.5f);
    sf::FloatRect lb = m_label.getLocalBounds();
    m_label.setOrigin(lb.left + lb.width * 0.5f, lb.top + lb.height * 0.5f);
    m_label.setPosition(position);
}

// ---------------------------------------------------------------------------
// Draw
// ---------------------------------------------------------------------------
void BerrySprite::draw(sf::RenderWindow& window) const
{
    if (m_selected) window.draw(m_glow);
    window.draw(m_circle);
    window.draw(m_label);
}

// ---------------------------------------------------------------------------
// Hit-testing
// ---------------------------------------------------------------------------
bool BerrySprite::contains(sf::Vector2f point) const
{
    sf::Vector2f p = m_circle.getPosition();
    float dx = point.x - p.x, dy = point.y - p.y;
    return dx * dx + dy * dy <= RADIUS * RADIUS;
}

sf::Vector2f BerrySprite::getPosition() const { return m_circle.getPosition(); }
BerryType    BerrySprite::getType()     const { return m_type; }

void BerrySprite::setSelected(bool s) { m_selected = s; }
bool BerrySprite::isSelected()  const { return m_selected; }
