#include "ShopOverlay.h"
#include "AssetManager.h"
#include "BoardManager.h"

#include <array>
#include <string>

// ---------------------------------------------------------------------------
// Panel geometry
// ---------------------------------------------------------------------------
static constexpr float PX = 180.f, PY =  75.f;
static constexpr float PW = 920.f, PH = 570.f;

static const sf::Color OVERLAY_BG { 0,   0,   0, 165 };
static const sf::Color PANEL_BG   {12,  14,  32, 248 };
static const sf::Color PANEL_LINE {70,  90, 180 };
static const sf::Color ROW_EVEN   {22,  27,  52, 255 };
static const sf::Color ROW_ODD    {17,  22,  44, 255 };
static const sf::Color BUY_OK     {35, 155,  70 };
static const sf::Color BUY_NO     {70,  70,  70 };
static const sf::Color GOLD_TEXT  {255, 215,   0 };
static const sf::Color RED_TEXT   {200,  70,  70 };

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
ShopOverlay::ShopOverlay(AssetManager& assets) : m_assets(&assets)
{
    m_overlay.setSize({1280.f, 720.f});
    m_overlay.setFillColor(OVERLAY_BG);

    m_panel.setSize({PW, PH});
    m_panel.setPosition(PX, PY);
    m_panel.setFillColor(PANEL_BG);
    m_panel.setOutlineColor(PANEL_LINE);
    m_panel.setOutlineThickness(2.f);

    // Title
    m_title.setFont(assets.getFont("main"));
    m_title.setString("PIGEON SHOP");
    m_title.setCharacterSize(30);
    m_title.setFillColor(GOLD_TEXT);
    m_title.setStyle(sf::Text::Bold);
    m_title.setPosition(PX + 22.f, PY + 14.f);

    // Close button
    m_closeBtn.setSize({42.f, 42.f});
    m_closeBtn.setPosition(PX + PW - 52.f, PY + 10.f);
    m_closeBtn.setFillColor(sf::Color(170, 35, 35));

    m_closeLabel.setFont(assets.getFont("main"));
    m_closeLabel.setString("X");
    m_closeLabel.setCharacterSize(22);
    m_closeLabel.setFillColor(sf::Color::White);
    m_closeLabel.setStyle(sf::Text::Bold);
    sf::FloatRect cl = m_closeLabel.getLocalBounds();
    m_closeLabel.setOrigin(cl.left + cl.width  * 0.5f,
                           cl.top  + cl.height * 0.5f);
    m_closeLabel.setPosition(PX + PW - 31.f, PY + 31.f);

    // Section headers (position set later in rebuildRows)
    m_sectionPigeons.setFont(assets.getFont("main"));
    m_sectionPigeons.setString("-- PIGEONS --");
    m_sectionPigeons.setCharacterSize(17);
    m_sectionPigeons.setFillColor(sf::Color(160, 190, 255));

    m_sectionBerries.setFont(assets.getFont("main"));
    m_sectionBerries.setString("-- BERRIES --");
    m_sectionBerries.setCharacterSize(17);
    m_sectionBerries.setFillColor(sf::Color(200, 160, 255));
}

// ---------------------------------------------------------------------------
// Open / close
// ---------------------------------------------------------------------------
void ShopOverlay::open()  { m_open = true;  m_dirty = true; }
void ShopOverlay::close() { m_open = false; }
bool ShopOverlay::isOpen() const { return m_open; }

// ---------------------------------------------------------------------------
// Row building (called lazily before draw/click)
// ---------------------------------------------------------------------------
void ShopOverlay::rebuildRows(const BoardManager& board) const
{
    m_rows.clear();

    float rowY  = PY + 62.f;
    const float ROW_H = 54.f;
    const float ROW_W = PW - 24.f;
    int rowIdx  = 0;

    auto makeText = [&](const std::string& str, unsigned sz, sf::Color col) {
        sf::Text t;
        t.setFont(m_assets->getFont("main"));
        t.setString(str);
        t.setCharacterSize(sz);
        t.setFillColor(col);
        return t;
    };

    // ── Pigeon section ───────────────────────────────────────────────────────
    m_sectionPigeons.setPosition(PX + 18.f, rowY);
    rowY += 26.f;

    // Tier colours for the icon circles (same palette as AssetManager)
    static const std::array<sf::Color, 16> TC = {{
        {135,206,250},{70,130,180},{60,179,113},{154,205,50},
        {255,215,0},  {255,140,0}, {255,69,0},  {220,20,60},
        {199,21,133}, {128,0,128}, {75,0,130},  {148,0,211},
        {0,128,128},  {0,206,209}, {212,175,55},{255,255,255},
    }};

    auto pigeonOffers = board.getAvailablePigeonOffers();
    for (const auto& offer : pigeonOffers) {
        if (rowY > PY + PH - ROW_H) break;

        ShopRow row;
        row.isBerry    = false;
        row.pigeonTier = offer.tier;
        row.canAfford  = (board.getCoins() >= offer.currentPrice);

        row.bg.setSize({ROW_W, ROW_H});
        row.bg.setPosition(PX + 12.f, rowY);
        row.bg.setFillColor((rowIdx % 2 == 0) ? ROW_EVEN : ROW_ODD);

        // Colour icon
        int tc_idx = std::clamp(offer.tier - 1, 0, 15);
        row.icon.setRadius(20.f);
        row.icon.setOrigin(20.f, 20.f);
        row.icon.setFillColor(TC[static_cast<std::size_t>(tc_idx)]);
        row.icon.setPosition(PX + 32.f, rowY + ROW_H * 0.5f);

        // Name  (tier in parentheses)
        row.nameText = makeText("T" + std::to_string(offer.tier) + "  " + offer.name,
                                17, sf::Color(215, 228, 255));
        row.nameText.setPosition(PX + 62.f, rowY + 8.f);

        // Price
        row.priceText = makeText(std::to_string(offer.currentPrice) + " coins",
                                  15, row.canAfford ? GOLD_TEXT : RED_TEXT);
        row.priceText.setPosition(PX + 62.f, rowY + 29.f);

        // Buy button
        row.buyBtn.setSize({108.f, 36.f});
        row.buyBtn.setPosition(PX + PW - 138.f, rowY + (ROW_H - 36.f) * 0.5f);
        row.buyBtn.setFillColor(row.canAfford ? BUY_OK : BUY_NO);

        row.buyLabel = makeText(row.canAfford ? "BUY" : "N/A", 16, sf::Color::White);
        sf::FloatRect bl = row.buyLabel.getLocalBounds();
        row.buyLabel.setOrigin(bl.left + bl.width * 0.5f, bl.top + bl.height * 0.5f);
        row.buyLabel.setPosition(PX + PW - 84.f, rowY + ROW_H * 0.5f);

        m_rows.push_back(std::move(row));
        rowY  += ROW_H + 3.f;
        rowIdx++;
    }

    // ── Berry section ────────────────────────────────────────────────────────
    if (rowY + 32.f < PY + PH) {
        m_sectionBerries.setPosition(PX + 18.f, rowY);
        rowY += 26.f;

        static const sf::Color BERRY_COLORS[3] = {
            {220, 55, 55},   // Red
            {255, 220, 0},   // Yellow
            {155, 0, 200},   // Purple
        };

        auto berryOffers = board.getBerryOffers();
        for (const auto& offer : berryOffers) {
            if (rowY > PY + PH - ROW_H) break;

            ShopRow row;
            row.isBerry   = true;
            row.berryType = static_cast<int>(offer.type);
            row.canAfford = (board.getCoins() >= offer.price);

            row.bg.setSize({ROW_W, ROW_H});
            row.bg.setPosition(PX + 12.f, rowY);
            row.bg.setFillColor((rowIdx % 2 == 0) ? ROW_EVEN : ROW_ODD);

            int bIdx = static_cast<int>(offer.type) - 1;
            if (bIdx < 0 || bIdx > 2) bIdx = 0;
            row.icon.setRadius(20.f);
            row.icon.setOrigin(20.f, 20.f);
            row.icon.setFillColor(BERRY_COLORS[bIdx]);
            row.icon.setPosition(PX + 32.f, rowY + ROW_H * 0.5f);

            row.nameText = makeText(offer.name, 17, sf::Color(215, 228, 255));
            row.nameText.setPosition(PX + 62.f, rowY + 8.f);

            row.priceText = makeText(std::to_string(offer.price) + " coins",
                                      15, row.canAfford ? GOLD_TEXT : RED_TEXT);
            row.priceText.setPosition(PX + 62.f, rowY + 29.f);

            row.buyBtn.setSize({108.f, 36.f});
            row.buyBtn.setPosition(PX + PW - 138.f, rowY + (ROW_H - 36.f) * 0.5f);
            row.buyBtn.setFillColor(row.canAfford ? BUY_OK : BUY_NO);

            row.buyLabel = makeText(row.canAfford ? "BUY" : "N/A", 16, sf::Color::White);
            sf::FloatRect bl = row.buyLabel.getLocalBounds();
            row.buyLabel.setOrigin(bl.left + bl.width * 0.5f, bl.top + bl.height * 0.5f);
            row.buyLabel.setPosition(PX + PW - 84.f, rowY + ROW_H * 0.5f);

            m_rows.push_back(std::move(row));
            rowY  += ROW_H + 3.f;
            rowIdx++;
        }
    }

    m_dirty = false;
}

// ---------------------------------------------------------------------------
// Input
// ---------------------------------------------------------------------------
bool ShopOverlay::handleClick(sf::Vector2f pos, BoardManager& board)
{
    if (!m_open) return false;

    if (m_closeBtn.getGlobalBounds().contains(pos)) { close(); return true; }

    rebuildRows(board); // rebuild with fresh affordability

    for (const auto& row : m_rows) {
        if (row.buyBtn.getGlobalBounds().contains(pos) && row.canAfford) {
            if (row.isBerry)
                board.buyNewBerry(row.berryType);
            else
                board.buyNewPigeon(row.pigeonTier);
            m_dirty = true;
            return true;
        }
    }

    // Consume any click inside the panel; click outside closes
    if (!m_panel.getGlobalBounds().contains(pos)) close();
    return true;
}

// ---------------------------------------------------------------------------
// Draw
// ---------------------------------------------------------------------------
void ShopOverlay::draw(sf::RenderWindow& window, const BoardManager& board) const
{
    if (!m_open) return;
    if (m_dirty) rebuildRows(board);

    window.draw(m_overlay);
    window.draw(m_panel);
    window.draw(m_title);
    window.draw(m_closeBtn);
    window.draw(m_closeLabel);
    window.draw(m_sectionPigeons);

    bool berryHeaderDrawn = false;
    for (const auto& row : m_rows) {
        if (row.isBerry && !berryHeaderDrawn) {
            window.draw(m_sectionBerries);
            berryHeaderDrawn = true;
        }
        window.draw(row.bg);
        window.draw(row.icon);
        window.draw(row.nameText);
        window.draw(row.priceText);
        window.draw(row.buyBtn);
        window.draw(row.buyLabel);
    }
}
