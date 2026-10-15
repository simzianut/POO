#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "AssetManager.h"
#include "EntitySprite.h"
#include "CrateSprite.h"
#include "BerrySprite.h"
#include "FloatingText.h"
#include "HudBar.h"
#include "ShopOverlay.h"
#include "EncyclopediaOverlay.h"
#include "Berry.h"

class Pigeon;

/// Top-level controller: owns the window, all visual objects, and the game loop.
/// Bridges SFML events to BoardManager calls without touching backend internals.
class GameWindow {
public:
    GameWindow();
    void run();

private:
    // ── Layout ───────────────────────────────────────────────────────────────
    static constexpr unsigned WIN_W        = 1280;
    static constexpr unsigned WIN_H        = 720;
    static constexpr float    HUD_TOP_H   = 70.f;
    static constexpr float    HUD_BOT_H   = 70.f;
    static constexpr float    BERRY_TRAY_W = 90.f;   ///< right-side berry panel width
    static constexpr int      MAX_BOARD_ITEMS = 24;
    static constexpr float    CRATE_SPAWN_INTERVAL_SECONDS = 6.f;

    /// Full render area between the two HUD bars.
    [[nodiscard]] sf::FloatRect playArea() const;
    /// Play area minus the berry tray on the right (for pigeon wandering).
    [[nodiscard]] sf::FloatRect entityPlayArea() const;
    /// Fixed tray position for each berry type.
    [[nodiscard]] sf::Vector2f  berryPositionForType(BerryType type) const;

    // ── Event layer ──────────────────────────────────────────────────────────
    void processEvents();
    void onMousePressed(sf::Vector2i pxPos);
    void onMouseMoved(sf::Vector2i pxPos);
    void onMouseReleased(sf::Vector2i pxPos);

    // ── Logic layer ──────────────────────────────────────────────────────────
    void update(float dt);

    /// Attempt a drag-and-drop merge between entity at idxA and entity at idxB.
    void handleMerge(int idxA, int idxB);

    /// Snap entity[idx] to the nearest free grid cell in the entity play area.
    void snapToField(int entityIdx);

    /// Reconcile m_entities with BoardManager::getActivePigeons().
    void syncEntitiesWithBoard();

    /// Find an open board position for a new crate, if one is available.
    [[nodiscard]] std::optional<sf::Vector2f> findCrateSpawnPosition() const;
    void openCrateAt(std::size_t crateIndex);

    /// Add a new EntitySprite for p at a random position inside entityPlayArea().
    void spawnEntityForPigeon(Pigeon* p);
    /// Add a new EntitySprite for p at the given board position.
    void spawnEntityForPigeon(Pigeon* p, sf::Vector2f position);

    /// Reconcile m_berrySprites with current berry inventory (max 1 per type).
    void syncBerriesWithBoard();

    // ── VFX helpers ──────────────────────────────────────────────────────────
    void spawnFloatingText(sf::Vector2f        pos,
                           const std::string&  msg,
                           sf::Color           col      = sf::Color(255, 215, 0),
                           unsigned            charSize = 22);

    /// Tick cosmetic poop-drop sprites (purely visual, no game logic).
    void tickPoopFX(float dt);

    // ── Render layer ─────────────────────────────────────────────────────────
    void render();
    void drawBackground();
    void drawBerryTray();
    void drawPoopFX();
    void drawEntities();
    void drawCrates();
    void drawFloatingTexts();

    // ── Core members (declaration order matters for constructor init order) ───
    sf::RenderWindow     m_window;
    sf::Clock            m_clock;
    AssetManager         m_assets;          ///< must be before m_hud / overlays

    std::vector<EntitySprite>  m_entities;
    std::vector<CrateSprite>   m_crates;
    std::vector<FloatingText>  m_floatingTexts;

    HudBar               m_hud;
    ShopOverlay          m_shopOverlay;
    EncyclopediaOverlay  m_encOverlay;

    /// Index into m_entities of the currently dragged entity, or -1.
    int          m_draggedIdx = -1;
    sf::Vector2f m_dragOffset;

    int          m_lastCoins  = 0;
    float        m_crateSpawnTimer = 0.f;

    // Background
    sf::Sprite   m_bgSprite;

    // ── Berry tray ────────────────────────────────────────────────────────────
    std::vector<BerrySprite>  m_berrySprites;
    BerryType                 m_selectedBerryType = BerryType::None;
    sf::RectangleShape        m_berryTrayBg;
    sf::Text                  m_berryTrayLabel;
    sf::Text                  m_berryHintText;

    // Cosmetic poop FX
    struct PoopFX {
        sf::CircleShape shape;
        sf::Vector2f    velocity;
        float           elapsed  = 0.f;
        float           lifetime = 2.0f;
    };
    std::vector<PoopFX> m_poopFX;
    float               m_poopTimer = 0.f;
};
