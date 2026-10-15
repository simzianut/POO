#include "GameWindow.h"
#include "BoardManager.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <string>

// ---------------------------------------------------------------------------
// Local helpers
// ---------------------------------------------------------------------------
static float randF(float lo, float hi)
{
    return lo + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (hi - lo);
}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
GameWindow::GameWindow()
    : m_window(sf::VideoMode(WIN_W, WIN_H), "Pigeon Evolution",
               sf::Style::Titlebar | sf::Style::Close)
    , m_hud(m_assets)
    , m_shopOverlay(m_assets)
    , m_encOverlay(m_assets)
{
    m_window.setFramerateLimit(60);

    // Background sprite
    sf::Texture& bgTex = m_assets.getTexture("background");
    m_bgSprite.setTexture(bgTex);
    float sx = static_cast<float>(WIN_W) / static_cast<float>(bgTex.getSize().x);
    float sy = static_cast<float>(WIN_H) / static_cast<float>(bgTex.getSize().y);
    m_bgSprite.setScale(sx, sy);

    // ── Berry tray panel ─────────────────────────────────────────────────────
    float trayX = static_cast<float>(WIN_W) - BERRY_TRAY_W;
    float trayH = static_cast<float>(WIN_H) - HUD_TOP_H - HUD_BOT_H;

    m_berryTrayBg.setSize({BERRY_TRAY_W, trayH});
    m_berryTrayBg.setPosition(trayX, HUD_TOP_H);
    m_berryTrayBg.setFillColor(sf::Color(10, 10, 26, 215));
    m_berryTrayBg.setOutlineColor(sf::Color(50, 50, 100));
    m_berryTrayBg.setOutlineThickness(1.f);

    // "BERRIES" title
    m_berryTrayLabel.setFont(m_assets.getFont("main"));
    m_berryTrayLabel.setString("BERRIES");
    m_berryTrayLabel.setCharacterSize(11);
    m_berryTrayLabel.setFillColor(sf::Color(130, 130, 200));
    m_berryTrayLabel.setStyle(sf::Text::Bold);
    {
        sf::FloatRect lb = m_berryTrayLabel.getLocalBounds();
        m_berryTrayLabel.setOrigin(lb.left + lb.width  * 0.5f,
                                   lb.top  + lb.height * 0.5f);
    }
    m_berryTrayLabel.setPosition(trayX + BERRY_TRAY_W * 0.5f, HUD_TOP_H + 14.f);

    // Hint text
    m_berryHintText.setFont(m_assets.getFont("main"));
    m_berryHintText.setString("click berry\nthen pigeon");
    m_berryHintText.setCharacterSize(10);
    m_berryHintText.setFillColor(sf::Color(100, 100, 160));
    m_berryHintText.setPosition(trayX + 4.f, HUD_TOP_H + trayH - 46.f);

    // ── Initial state ─────────────────────────────────────────────────────────
    BoardManager& board = BoardManager::getInstance();
    board.spawnBabyPigeon(1);
    syncEntitiesWithBoard();
    m_lastCoins = board.getCoins();
}

// ---------------------------------------------------------------------------
// Layout helpers
// ---------------------------------------------------------------------------
sf::FloatRect GameWindow::playArea() const
{
    return { 0.f, HUD_TOP_H,
             static_cast<float>(WIN_W),
             static_cast<float>(WIN_H) - HUD_TOP_H - HUD_BOT_H };
}

sf::FloatRect GameWindow::entityPlayArea() const
{
    // Entities stay out of the berry tray column
    return { 0.f, HUD_TOP_H,
             static_cast<float>(WIN_W) - BERRY_TRAY_W - 5.f,
             static_cast<float>(WIN_H) - HUD_TOP_H - HUD_BOT_H };
}

sf::Vector2f GameWindow::berryPositionForType(BerryType type) const
{
    const float cx = static_cast<float>(WIN_W) - BERRY_TRAY_W * 0.5f;
    switch (type) {
        case BerryType::Red:    return { cx, HUD_TOP_H + 90.f  };
        case BerryType::Yellow: return { cx, HUD_TOP_H + 210.f };
        case BerryType::Purple: return { cx, HUD_TOP_H + 330.f };
        default:                return { cx, HUD_TOP_H + 210.f };
    }
}

// ---------------------------------------------------------------------------
// Run
// ---------------------------------------------------------------------------
void GameWindow::run()
{
    while (m_window.isOpen()) {
        float dt = m_clock.restart().asSeconds();
        dt = std::min(dt, 0.05f);
        processEvents();
        update(dt);
        render();
    }
}

// ============================================================================
// Event layer
// ============================================================================
void GameWindow::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            m_window.close();
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left)
                onMousePressed({ event.mouseButton.x, event.mouseButton.y });
            break;
        case sf::Event::MouseMoved:
            onMouseMoved({ event.mouseMove.x, event.mouseMove.y });
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left)
                onMouseReleased({ event.mouseButton.x, event.mouseButton.y });
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape) m_window.close();
            break;
        default:
            break;
        }
    }
}

// ---------------------------------------------------------------------------
void GameWindow::onMousePressed(sf::Vector2i pxPos)
{
    sf::Vector2f pos = m_window.mapPixelToCoords(pxPos);

    // ── Overlays consume all clicks while open ───────────────────────────────
    if (m_shopOverlay.isOpen() || m_encOverlay.isOpen()) {
        BoardManager& board = BoardManager::getInstance();
        if (m_shopOverlay.isOpen()) {
            m_shopOverlay.handleClick(pos, board);
            syncEntitiesWithBoard();
            syncBerriesWithBoard();
        } else {
            m_encOverlay.handleClick(pos);
        }
        return;
    }

    // ── HUD buttons ──────────────────────────────────────────────────────────
    // Crates render above pigeons, so let a crate consume the click first.
    for (int i = static_cast<int>(m_crates.size()) - 1; i >= 0; --i) {
        const std::size_t crateIndex = static_cast<std::size_t>(i);
        if (m_crates[crateIndex].contains(pos)) {
            if (m_selectedBerryType != BerryType::None) {
                m_selectedBerryType = BerryType::None;
                for (auto& berry : m_berrySprites) berry.setSelected(false);
            }
            openCrateAt(crateIndex);
            return;
        }
    }

    HudAction action = m_hud.handleClick(pos);
    if (action != HudAction::None) {
        switch (action) {
        case HudAction::Shop:
            m_shopOverlay.open();
            break;
        case HudAction::Encyclopedia:
            m_encOverlay.open();
            break;
        default: break;
        }
        return;
    }

    // ── Berry interaction ─────────────────────────────────────────────────────
    // Step 1: if a berry is selected and user clicks an entity → feed it
    if (m_selectedBerryType != BerryType::None) {
        for (int i = static_cast<int>(m_entities.size()) - 1; i >= 0; --i) {
            if (m_entities[static_cast<std::size_t>(i)].contains(pos)) {
                BoardManager& board = BoardManager::getInstance();
                int pigeonIdx = board.getPigeonIndex(
                    m_entities[static_cast<std::size_t>(i)].getPigeon());
                if (pigeonIdx >= 0) {
                    int before = board.getBerryCount(m_selectedBerryType);
                    board.feedBerry(static_cast<int>(m_selectedBerryType), pigeonIdx);
                    int after  = board.getBerryCount(m_selectedBerryType);

                    sf::Vector2f ppos = m_entities[static_cast<std::size_t>(i)].getPosition();
                    if (after < before) {
                        spawnFloatingText(ppos - sf::Vector2f(0.f, 55.f),
                                          "Berry fed!", sf::Color(210, 120, 255));
                    } else {
                        spawnFloatingText(ppos - sf::Vector2f(0.f, 55.f),
                                          "Can't feed!", sf::Color(255, 80, 80));
                    }
                    syncBerriesWithBoard();
                }
                // Always deselect after click on pigeon
                m_selectedBerryType = BerryType::None;
                for (auto& bs : m_berrySprites) bs.setSelected(false);
                return;
            }
        }
    }

    // Step 2: clicking a berry sprite → select / deselect
    for (auto& bs : m_berrySprites) {
        if (bs.contains(pos)) {
            if (m_selectedBerryType == bs.getType()) {
                // Same berry again → deselect
                m_selectedBerryType = BerryType::None;
                bs.setSelected(false);
            } else {
                m_selectedBerryType = bs.getType();
                for (auto& other : m_berrySprites) other.setSelected(false);
                bs.setSelected(true);
                spawnFloatingText(bs.getPosition() - sf::Vector2f(0.f, 42.f),
                                  "Click a pigeon!", sf::Color(210, 180, 255), 16);
            }
            return;
        }
    }

    // Step 3: clicked elsewhere while berry selected → deselect
    if (m_selectedBerryType != BerryType::None) {
        m_selectedBerryType = BerryType::None;
        for (auto& bs : m_berrySprites) bs.setSelected(false);
    }

    // ── Pick up an entity (iterate reverse for Z-order) ───────────────────────
    for (int i = static_cast<int>(m_entities.size()) - 1; i >= 0; --i) {
        if (m_entities[static_cast<std::size_t>(i)].contains(pos)) {
            m_draggedIdx = i;
            m_dragOffset = pos - m_entities[static_cast<std::size_t>(i)].getPosition();
            return;
        }
    }
}

// ---------------------------------------------------------------------------
void GameWindow::onMouseMoved(sf::Vector2i pxPos)
{
    sf::Vector2f pos = m_window.mapPixelToCoords(pxPos);
    m_hud.update(pos, BoardManager::getInstance().getCoins());

    if (m_draggedIdx >= 0 && m_draggedIdx < static_cast<int>(m_entities.size()))
        m_entities[static_cast<std::size_t>(m_draggedIdx)].setPosition(pos - m_dragOffset);
}

// ---------------------------------------------------------------------------
void GameWindow::onMouseReleased(sf::Vector2i)
{
    if (m_draggedIdx < 0) return;

    const int droppedIdx   = m_draggedIdx;
    m_draggedIdx           = -1;

    if (m_shopOverlay.isOpen() || m_encOverlay.isOpen()) {
        snapToField(droppedIdx);
        return;
    }

    // Merge detection
    sf::FloatRect droppedBounds = m_entities[static_cast<std::size_t>(droppedIdx)].getBounds();
    for (int i = 0; i < static_cast<int>(m_entities.size()); ++i) {
        if (i == droppedIdx) continue;
        if (droppedBounds.intersects(m_entities[static_cast<std::size_t>(i)].getBounds())) {
            handleMerge(droppedIdx, i);
            return;
        }
    }
    snapToField(droppedIdx);
}

// ============================================================================
// Logic layer
// ============================================================================
void GameWindow::update(float dt)
{
    BoardManager& board = BoardManager::getInstance();
    board.update();

    m_crateSpawnTimer += dt;
    if (m_crateSpawnTimer >= CRATE_SPAWN_INTERVAL_SECONDS) {
        m_crateSpawnTimer -= CRATE_SPAWN_INTERVAL_SECONDS;
        if (board.getTotalPigeonsAlive() + static_cast<int>(m_crates.size()) < MAX_BOARD_ITEMS) {
            if (const auto position = findCrateSpawnPosition())
                m_crates.emplace_back(*position);
        }
    }

    // Coin gain → floating "+N" text
    int currentCoins = board.getCoins();
    if (currentCoins > m_lastCoins && !m_entities.empty()) {
        int gained = currentCoins - m_lastCoins;
        std::size_t idx = static_cast<std::size_t>(rand()) % m_entities.size();
        sf::Vector2f p  = m_entities[idx].getPosition();
        spawnFloatingText(p + sf::Vector2f(0.f, -55.f),
                          "+" + std::to_string(gained));
    }
    m_lastCoins = currentCoins;

    // Sync both entity and berry lists (catches auto-merges, purchases, etc.)
    if (m_draggedIdx < 0) {
        syncEntitiesWithBoard();
        syncBerriesWithBoard();
    }

    // Update entity wander AI (skip dragged entity)
    for (int i = 0; i < static_cast<int>(m_entities.size()); ++i)
        if (i != m_draggedIdx)
            m_entities[static_cast<std::size_t>(i)].update(dt);

    // Update floating texts
    for (auto& ft : m_floatingTexts) ft.update(dt);
    m_floatingTexts.erase(
        std::remove_if(m_floatingTexts.begin(), m_floatingTexts.end(),
                       [](const FloatingText& f) { return f.isDead(); }),
        m_floatingTexts.end());

    tickPoopFX(dt);
}

// ---------------------------------------------------------------------------
void GameWindow::handleMerge(int idxA, int idxB)
{
    Pigeon*      pa       = m_entities[static_cast<std::size_t>(idxA)].getPigeon();
    Pigeon*      pb       = m_entities[static_cast<std::size_t>(idxB)].getPigeon();
    sf::Vector2f mergePos = m_entities[static_cast<std::size_t>(idxA)].getPosition();

    BoardManager& board   = BoardManager::getInstance();
    int boardIdxA         = board.getPigeonIndex(pa);
    int boardIdxB         = board.getPigeonIndex(pb);

    if (boardIdxA < 0 || boardIdxB < 0) { snapToField(idxA); return; }

    const int prevSize = static_cast<int>(board.getActivePigeons().size());
    board.performMerge(boardIdxA, boardIdxB);
    const int newSize  = static_cast<int>(board.getActivePigeons().size());

    if (newSize < prevSize) {
        // ── Success ───────────────────────────────────────────────────────────
        Pigeon* evolved = board.getActivePigeons().back();
        int lo = std::min(idxA, idxB);
        int hi = std::max(idxA, idxB);
        m_entities.erase(m_entities.begin() + hi);
        m_entities.erase(m_entities.begin() + lo);
        spawnEntityForPigeon(evolved);
        m_entities.back().setPosition(mergePos);
        m_entities.back().triggerPopAnimation();
        spawnFloatingText(mergePos + sf::Vector2f(0.f, -60.f),
                          "EVOLVED!", sf::Color(255, 210, 50));

        int gained = board.getCoins() - m_lastCoins;
        if (gained > 0) {
            spawnFloatingText(mergePos + sf::Vector2f(40.f, -30.f),
                              "+" + std::to_string(gained));
            m_lastCoins = board.getCoins();
        }
    } else {
        // ── Failure ───────────────────────────────────────────────────────────
        snapToField(idxA);
        spawnFloatingText(m_entities[static_cast<std::size_t>(idxA)].getPosition()
                              + sf::Vector2f(0.f, -45.f),
                          "Can't merge!", sf::Color(255, 80, 80));
    }
}

// ---------------------------------------------------------------------------
void GameWindow::snapToField(int entityIdx)
{
    if (entityIdx < 0 || entityIdx >= static_cast<int>(m_entities.size())) return;

    const sf::FloatRect area = entityPlayArea();
    constexpr float CELL_SIZE = 100.f;
    constexpr float RADIUS    = 40.f;

    const int cols = static_cast<int>(area.width  / CELL_SIZE);
    const int rows = static_cast<int>(area.height / CELL_SIZE);

    sf::Vector2f bestPos  = m_entities[static_cast<std::size_t>(entityIdx)].getPosition();
    float        bestDist = std::numeric_limits<float>::max();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            float cx = area.left + static_cast<float>(c) * CELL_SIZE + CELL_SIZE * 0.5f;
            float cy = area.top  + static_cast<float>(r) * CELL_SIZE + CELL_SIZE * 0.5f;
            cx = std::clamp(cx, area.left + RADIUS, area.left + area.width  - RADIUS);
            cy = std::clamp(cy, area.top  + RADIUS, area.top  + area.height - RADIUS);

            bool occupied = false;
            for (int k = 0; k < static_cast<int>(m_entities.size()); ++k) {
                if (k == entityIdx) continue;
                sf::Vector2f op = m_entities[static_cast<std::size_t>(k)].getPosition();
                if (std::abs(op.x - cx) < 20.f && std::abs(op.y - cy) < 20.f) {
                    occupied = true; break;
                }
            }

            if (!occupied) {
                sf::Vector2f ep = m_entities[static_cast<std::size_t>(entityIdx)].getPosition();
                float dx = cx - ep.x, dy = cy - ep.y;
                float d  = dx * dx + dy * dy;
                if (d < bestDist) { bestDist = d; bestPos = {cx, cy}; }
            }
        }
    }
    m_entities[static_cast<std::size_t>(entityIdx)].setPosition(bestPos);
}

// ---------------------------------------------------------------------------
void GameWindow::syncEntitiesWithBoard()
{
    BoardManager&       board         = BoardManager::getInstance();
    const auto&         activePigeons = board.getActivePigeons();

    for (Pigeon* p : activePigeons) {
        bool found = false;
        for (const auto& e : m_entities) if (e.getPigeon() == p) { found = true; break; }
        if (!found) spawnEntityForPigeon(p);
    }

    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(),
            [&](const EntitySprite& e) {
                return std::find(activePigeons.begin(), activePigeons.end(),
                                 e.getPigeon()) == activePigeons.end();
            }),
        m_entities.end());
}

// ---------------------------------------------------------------------------
std::optional<sf::Vector2f> GameWindow::findCrateSpawnPosition() const
{
    const sf::FloatRect area = entityPlayArea();
    for (int attempt = 0; attempt < 100; ++attempt) {
        const sf::Vector2f position{
            randF(area.left + 42.f, area.left + area.width - 54.f),
            randF(area.top + 40.f, area.top + area.height - 45.f)
        };

        // Leave a little visual breathing room around the box.
        const sf::FloatRect candidate{
            position.x - 40.f, position.y - 37.f, 92.f, 82.f
        };
        bool occupied = false;
        for (const auto& entity : m_entities) {
            if (candidate.intersects(entity.getBounds())) {
                occupied = true;
                break;
            }
        }
        if (occupied)
            continue;

        for (const auto& crate : m_crates) {
            if (candidate.intersects(crate.getBounds())) {
                occupied = true;
                break;
            }
        }
        if (!occupied)
            return position;
    }
    return std::nullopt;
}

void GameWindow::openCrateAt(const std::size_t crateIndex)
{
    if (crateIndex >= m_crates.size())
        return;

    const sf::Vector2f position = m_crates[crateIndex].getPosition();
    BoardManager& board = BoardManager::getInstance();
    Pigeon* pigeon = board.openCrate();
    if (pigeon == nullptr)
        return;

    m_crates.erase(m_crates.begin() + static_cast<std::ptrdiff_t>(crateIndex));
    spawnEntityForPigeon(pigeon, position);
    m_entities.back().triggerPopAnimation();
    syncEntitiesWithBoard();
    spawnFloatingText(position - sf::Vector2f(0.f, 48.f),
                      "Opened!", sf::Color(255, 210, 120), 16);
}

void GameWindow::syncBerriesWithBoard()
{
    BoardManager& board = BoardManager::getInstance();
    static const BerryType TYPES[3] = {
        BerryType::Red, BerryType::Yellow, BerryType::Purple
    };

    for (BerryType type : TYPES) {
        int  count     = board.getBerryCount(type);
        bool hasSprite = false;
        for (const auto& bs : m_berrySprites)
            if (bs.getType() == type) { hasSprite = true; break; }

        if (count > 0 && !hasSprite) {
            // Berry in inventory but no sprite yet → spawn one
            m_berrySprites.emplace_back(type, m_assets, berryPositionForType(type));
        } else if (count == 0 && hasSprite) {
            // Berry gone from inventory → remove sprite
            m_berrySprites.erase(
                std::remove_if(m_berrySprites.begin(), m_berrySprites.end(),
                    [type](const BerrySprite& bs) { return bs.getType() == type; }),
                m_berrySprites.end());
            if (m_selectedBerryType == type)
                m_selectedBerryType = BerryType::None;
        }
    }
}

// ---------------------------------------------------------------------------
void GameWindow::spawnEntityForPigeon(Pigeon* p)
{
    m_entities.emplace_back(p, m_assets, entityPlayArea());
}

void GameWindow::spawnEntityForPigeon(Pigeon* p, const sf::Vector2f position)
{
    m_entities.emplace_back(p, m_assets, entityPlayArea(), position);
}

// ============================================================================
// VFX helpers
// ============================================================================
void GameWindow::spawnFloatingText(sf::Vector2f       pos,
                                   const std::string& msg,
                                   sf::Color          col,
                                   unsigned           charSize)
{
    m_floatingTexts.emplace_back(m_assets.getFont("main"), msg, pos, col, charSize);
}

// ---------------------------------------------------------------------------
void GameWindow::tickPoopFX(float dt)
{
    m_poopTimer -= dt;
    if (m_poopTimer <= 0.f && !m_entities.empty()) {
        m_poopTimer = randF(0.7f, 1.8f);
        std::size_t  idx  = static_cast<std::size_t>(rand()) % m_entities.size();
        sf::Vector2f ppos = m_entities[idx].getPosition();

        PoopFX fx;
        fx.shape.setRadius(7.f);
        fx.shape.setOrigin(7.f, 7.f);
        fx.shape.setFillColor(sf::Color(101, 67, 33, 200));
        fx.shape.setPosition(ppos + sf::Vector2f(randF(-25.f, 25.f), 10.f));
        fx.velocity  = { randF(-12.f, 12.f), randF(-38.f, -18.f) };
        fx.elapsed   = 0.f;
        fx.lifetime  = 2.0f;
        m_poopFX.push_back(fx);
    }

    for (auto& fx : m_poopFX) {
        fx.elapsed += dt;
        fx.shape.move(fx.velocity * dt);
        float alpha = std::max(0.f, 1.f - fx.elapsed / fx.lifetime);
        sf::Color c = fx.shape.getFillColor();
        c.a = static_cast<sf::Uint8>(alpha * 200.f);
        fx.shape.setFillColor(c);
    }

    m_poopFX.erase(
        std::remove_if(m_poopFX.begin(), m_poopFX.end(),
                       [](const PoopFX& fx) { return fx.elapsed >= fx.lifetime; }),
        m_poopFX.end());
}

// ============================================================================
// Render layer
// ============================================================================
void GameWindow::render()
{
    m_window.clear(sf::Color(10, 10, 25));

    drawBackground();
    drawBerryTray();       // berry panel sits behind pigeons
    drawPoopFX();
    drawEntities();
    drawCrates();
    drawFloatingTexts();

    m_shopOverlay.draw(m_window, BoardManager::getInstance());
    m_encOverlay.draw(m_window, BoardManager::getInstance().getEncyclopedia());

    m_hud.draw(m_window);  // HUD always on top
    m_window.display();
}

// ---------------------------------------------------------------------------
void GameWindow::drawBackground() { m_window.draw(m_bgSprite); }

// ---------------------------------------------------------------------------
void GameWindow::drawBerryTray()
{
    m_window.draw(m_berryTrayBg);
    m_window.draw(m_berryTrayLabel);
    m_window.draw(m_berryHintText);
    for (const auto& bs : m_berrySprites) bs.draw(m_window);
}

// ---------------------------------------------------------------------------
void GameWindow::drawPoopFX()
{
    for (const auto& fx : m_poopFX) m_window.draw(fx.shape);
}

// ---------------------------------------------------------------------------
void GameWindow::drawEntities()
{
    for (int i = 0; i < static_cast<int>(m_entities.size()); ++i)
        if (i != m_draggedIdx)
            m_entities[static_cast<std::size_t>(i)].draw(m_window);

    if (m_draggedIdx >= 0 && m_draggedIdx < static_cast<int>(m_entities.size()))
        m_entities[static_cast<std::size_t>(m_draggedIdx)].draw(m_window);
}

// ---------------------------------------------------------------------------
void GameWindow::drawCrates()
{
    for (const auto& crate : m_crates)
        crate.draw(m_window);
}

void GameWindow::drawFloatingTexts()
{
    for (const auto& ft : m_floatingTexts) ft.draw(m_window);
}
