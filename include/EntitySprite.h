#pragma once

#include <SFML/Graphics.hpp>
#include "Pigeon.h"

class AssetManager;

/// Visual wrapper around a Pigeon*.
/// Renders a tier-illustrated pigeon that wanders idly inside the play area.
/// While being dragged the owner suspends calls to update().
class EntitySprite {
public:
    EntitySprite(Pigeon* pigeon, AssetManager& assets, sf::FloatRect playArea);
    EntitySprite(Pigeon* pigeon, AssetManager& assets, sf::FloatRect playArea,
                 sf::Vector2f initialPosition);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    [[nodiscard]] bool           contains(sf::Vector2f point) const;
    [[nodiscard]] sf::FloatRect  getBounds()   const;
    [[nodiscard]] sf::Vector2f   getPosition() const;
    [[nodiscard]] Pigeon*        getPigeon()   const;

    void setPosition(sf::Vector2f pos);

    /// Plays a quick scale-pop animation (triggered on successful merge).
    void triggerPopAnimation();

private:
    void pickNewWanderTarget();
    void clampToPlayArea();
    void updatePopAnimation(float dt);

    static constexpr float WANDER_SPEED  = 25.f;
    static constexpr float WANDER_PERIOD = 5.5f;
    static constexpr float ENTITY_RADIUS = 40.f;   ///< half of the 80×80 texture
    static constexpr float POP_DURATION  = 0.35f;

    Pigeon*       m_pigeon;
    sf::Sprite    m_sprite;
    sf::CircleShape m_tierBadge;
    sf::Text      m_tierLabel;
    sf::FloatRect m_playArea;
    sf::Vector2f  m_wanderTarget{};
    float         m_wanderTimer = 0.f;
    float         m_popTimer    = 0.f;
    float         m_baseScale   = 1.f;
    bool          m_popping     = false;
};
