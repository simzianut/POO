#include "EntitySprite.h"
#include "AssetManager.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
static float randF(float lo, float hi)
{
    return lo + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (hi - lo);
}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
EntitySprite::EntitySprite(Pigeon* pigeon, AssetManager& assets, sf::FloatRect playArea)
    : EntitySprite(pigeon, assets, playArea,
                   {randF(playArea.left + 55.f, playArea.left + playArea.width - 55.f),
                    randF(playArea.top + 55.f, playArea.top + playArea.height - 55.f)})
{}

EntitySprite::EntitySprite(Pigeon* pigeon, AssetManager& assets,
                           sf::FloatRect playArea, sf::Vector2f initialPosition)
    : m_pigeon(pigeon), m_playArea(playArea)
{
    // Sprite
    sf::Texture& tex = assets.getPigeonTexture(pigeon->getTier());
    m_sprite.setTexture(tex);
    m_sprite.setOrigin(ENTITY_RADIUS, ENTITY_RADIUS); // 80×80 tex → centre at 40,40

    // Tier label (small number drawn in the centre of the sprite)
    m_tierLabel.setFont(assets.getFont("main"));
    m_tierLabel.setString(std::to_string(pigeon->getTier()));
    m_tierLabel.setCharacterSize(16);
    m_tierLabel.setFillColor(sf::Color::White);
    m_tierLabel.setStyle(sf::Text::Bold);
    m_tierLabel.setOutlineColor(sf::Color::Black);
    m_tierLabel.setOutlineThickness(1.5f);
    sf::FloatRect lb = m_tierLabel.getLocalBounds();
    m_tierLabel.setOrigin(lb.left + lb.width  * 0.5f,
                          lb.top  + lb.height * 0.5f);

    setPosition(initialPosition);
    pickNewWanderTarget();
}

// ---------------------------------------------------------------------------
// Update
// ---------------------------------------------------------------------------
void EntitySprite::update(float dt)
{
    // Pop animation
    if (m_popping)
        updatePopAnimation(dt);

    // Wander
    m_wanderTimer -= dt;
    sf::Vector2f pos = m_sprite.getPosition();
    sf::Vector2f dir = m_wanderTarget - pos;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (dist > 3.f) {
        pos += (dir / dist) * WANDER_SPEED * dt;
        setPosition(pos);
    } else {
        pickNewWanderTarget();
    }

    if (m_wanderTimer <= 0.f)
        pickNewWanderTarget();

    clampToPlayArea();
}

void EntitySprite::updatePopAnimation(float dt)
{
    m_popTimer += dt;
    float t = std::min(m_popTimer / POP_DURATION, 1.f);

    // Scale: 1 → 1.5 → 1  (ease-in then ease-out)
    float scale = (t < 0.5f)
                  ? 1.f + t * 2.f * 0.5f          // 1 → 1.5
                  : 1.5f - (t - 0.5f) * 2.f * 0.5f; // 1.5 → 1

    m_sprite.setScale(scale, scale);

    if (t >= 1.f) {
        m_popping = false;
        m_sprite.setScale(1.f, 1.f);
    }
}

// ---------------------------------------------------------------------------
// Draw
// ---------------------------------------------------------------------------
void EntitySprite::draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
    window.draw(m_tierLabel);
}

// ---------------------------------------------------------------------------
// Hit-testing
// ---------------------------------------------------------------------------
bool EntitySprite::contains(sf::Vector2f point) const
{
    sf::Vector2f pos = m_sprite.getPosition();
    float dx = point.x - pos.x;
    float dy = point.y - pos.y;
    return (dx * dx + dy * dy) <= ENTITY_RADIUS * ENTITY_RADIUS;
}

sf::FloatRect EntitySprite::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

// ---------------------------------------------------------------------------
// Accessors / mutators
// ---------------------------------------------------------------------------
sf::Vector2f EntitySprite::getPosition() const { return m_sprite.getPosition(); }
Pigeon*      EntitySprite::getPigeon()   const { return m_pigeon; }

void EntitySprite::setPosition(sf::Vector2f pos)
{
    m_sprite.setPosition(pos);
    m_tierLabel.setPosition(pos);
}

void EntitySprite::triggerPopAnimation()
{
    m_popping  = true;
    m_popTimer = 0.f;
    m_sprite.setScale(1.f, 1.f);
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------
void EntitySprite::pickNewWanderTarget()
{
    m_wanderTimer  = WANDER_PERIOD + randF(-1.f, 1.2f);
    float x = randF(m_playArea.left   + 55.f, m_playArea.left   + m_playArea.width  - 55.f);
    float y = randF(m_playArea.top    + 55.f, m_playArea.top    + m_playArea.height - 55.f);
    m_wanderTarget = {x, y};
}

void EntitySprite::clampToPlayArea()
{
    sf::Vector2f pos  = m_sprite.getPosition();
    float minX = m_playArea.left  + ENTITY_RADIUS;
    float maxX = m_playArea.left  + m_playArea.width  - ENTITY_RADIUS;
    float minY = m_playArea.top   + ENTITY_RADIUS;
    float maxY = m_playArea.top   + m_playArea.height - ENTITY_RADIUS;

    pos.x = std::clamp(pos.x, minX, maxX);
    pos.y = std::clamp(pos.y, minY, maxY);
    setPosition(pos);
}
