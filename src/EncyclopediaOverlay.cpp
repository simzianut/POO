#include "EncyclopediaOverlay.h"
#include "AssetManager.h"

#include <cmath>
#include <string>

// ---------------------------------------------------------------------------
// Panel geometry
// ---------------------------------------------------------------------------
static constexpr float PX = 130.f, PY =  65.f;
static constexpr float PW = 1020.f, PH = 590.f;

static const sf::Color OVERLAY_BG { 0,   0,   0, 160 };
static const sf::Color PANEL_BG   { 9,  10,  24, 250 };
static const sf::Color PANEL_LINE {90,  70, 200 };
static const sf::Color ROW_EVEN   {18,  22,  44, 255 };
static const sf::Color ROW_ODD    {24,  29,  56, 255 };

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
EncyclopediaOverlay::EncyclopediaOverlay(AssetManager& assets) : m_assets(&assets)
{
    m_overlay.setSize({1280.f, 720.f});
    m_overlay.setFillColor(OVERLAY_BG);

    m_panel.setSize({PW, PH});
    m_panel.setPosition(PX, PY);
    m_panel.setFillColor(PANEL_BG);
    m_panel.setOutlineColor(PANEL_LINE);
    m_panel.setOutlineThickness(2.f);

    m_contentViewport = {PX + 12.f, PY + 56.f, PW - 40.f, PH - 72.f};
    m_scrollBar.configure({PX + PW - 22.f, PY + 56.f, 8.f, PH - 72.f},
                          0.f, m_contentViewport.height);

    m_title.setFont(assets.getFont("main"));
    m_title.setString("PIGEON ENCYCLOPEDIA");
    m_title.setCharacterSize(28);
    m_title.setFillColor(sf::Color(190, 160, 255));
    m_title.setStyle(sf::Text::Bold);
    m_title.setPosition(PX + 22.f, PY + 14.f);

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
}

// ---------------------------------------------------------------------------
// Open / close
// ---------------------------------------------------------------------------
void EncyclopediaOverlay::open()  { m_open = true; m_dirty = true; m_scrollBar.reset(); }
void EncyclopediaOverlay::close() { m_open = false; m_scrollBar.handleRelease(); }
bool EncyclopediaOverlay::isOpen() const { return m_open; }

// ---------------------------------------------------------------------------
// Row building
// ---------------------------------------------------------------------------
void EncyclopediaOverlay::rebuildRows(const Encyclopedia& enc)
{
    m_rows.clear();
    const auto& pigeons = enc.getPigeons();

    float rowY          = PY + 60.f;
    const float ROW_H   = 52.f;
    const float ROW_W   = m_contentViewport.width - 8.f;
    int rowIdx          = 0;

    auto makeText = [&](const std::string& str, unsigned sz, sf::Color col) {
        sf::Text t;
        t.setFont(m_assets->getFont("main"));
        t.setString(str);
        t.setCharacterSize(sz);
        t.setFillColor(col);
        return t;
    };

    for (const auto& info : pigeons) {
        EncRow row;
        row.bg.setSize({ROW_W, ROW_H});
        row.bg.setPosition(PX + 12.f, rowY);
        row.bg.setFillColor((rowIdx % 2 == 0) ? ROW_EVEN : ROW_ODD);

        row.nameText = makeText(info.name, 17, sf::Color(215, 228, 255));
        row.nameText.setStyle(sf::Text::Bold);
        row.nameText.setPosition(PX + 22.f, rowY + 6.f);

        // Format PPS
        std::string ppsStr;
        float pps = info.poopPerSecond;
        if      (pps >= 1000000.f) ppsStr = std::to_string(static_cast<int>(pps / 1000000.f)) + "M PPS";
        else if (pps >= 1000.f)    ppsStr = std::to_string(static_cast<int>(pps / 1000.f))    + "k PPS";
        else                       ppsStr = std::to_string(static_cast<int>(pps))              + " PPS";

        row.ppsText = makeText(ppsStr, 14, sf::Color(255, 215, 0));
        row.ppsText.setPosition(PX + 22.f, rowY + 28.f);

        row.descText = makeText(info.description, 14, sf::Color(155, 165, 190));
        row.descText.setPosition(PX + 240.f, rowY + 17.f);

        m_rows.push_back(std::move(row));
        rowY  += ROW_H + 2.f;
        rowIdx++;
    }

    const float contentHeight = rowY - m_contentViewport.top + 8.f;
    m_scrollBar.configure({PX + PW - 22.f, PY + 56.f, 8.f, PH - 72.f},
                          contentHeight, m_contentViewport.height);
    m_dirty = false;
}

// ---------------------------------------------------------------------------
// Input
// ---------------------------------------------------------------------------
bool EncyclopediaOverlay::handleClick(sf::Vector2f pos, const Encyclopedia& enc)
{
    if (!m_open) return false;
    if (m_closeBtn.getGlobalBounds().contains(pos)) { close(); return true; }
    if (m_dirty || m_rows.size() != enc.getPigeons().size()) rebuildRows(enc);
    if (m_scrollBar.handlePress(pos)) return true;
    if (!m_panel.getGlobalBounds().contains(pos)) close();
    return true;
}

bool EncyclopediaOverlay::handleMouseWheel(const sf::Vector2f pos, const float delta,
                                           const Encyclopedia& enc)
{
    if (!m_open) return false;
    if (m_dirty || m_rows.size() != enc.getPigeons().size()) rebuildRows(enc);
    return m_scrollBar.handleWheel(pos, delta, m_contentViewport);
}

bool EncyclopediaOverlay::handleMouseMoved(const sf::Vector2f pos, const Encyclopedia& enc)
{
    if (!m_open) return false;
    if (m_dirty || m_rows.size() != enc.getPigeons().size()) rebuildRows(enc);
    return m_scrollBar.handleMove(pos);
}

void EncyclopediaOverlay::handleMouseReleased()
{
    m_scrollBar.handleRelease();
}

// ---------------------------------------------------------------------------
// Draw
// ---------------------------------------------------------------------------
void EncyclopediaOverlay::draw(sf::RenderWindow& window, const Encyclopedia& enc)
{
    if (!m_open) return;
    if (m_dirty || m_rows.size() != enc.getPigeons().size()) rebuildRows(enc);

    window.draw(m_overlay);
    window.draw(m_panel);
    window.draw(m_title);
    window.draw(m_closeBtn);
    window.draw(m_closeLabel);

    const sf::View previousView = window.getView();
    sf::View contentView(sf::FloatRect(m_contentViewport.left,
                                       m_contentViewport.top + m_scrollBar.offset(),
                                       m_contentViewport.width,
                                       m_contentViewport.height));
    contentView.setViewport({m_contentViewport.left / 1280.f,
                             m_contentViewport.top / 720.f,
                             m_contentViewport.width / 1280.f,
                             m_contentViewport.height / 720.f});
    window.setView(contentView);
    for (const auto& row : m_rows) {
        window.draw(row.bg);
        window.draw(row.nameText);
        window.draw(row.ppsText);
        window.draw(row.descText);
    }
    window.setView(previousView);
    m_scrollBar.draw(window);
}
