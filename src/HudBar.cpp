#include "HudBar.h"
#include "AssetManager.h"

#include <string>

// ---------------------------------------------------------------------------
// Colour palette
// ---------------------------------------------------------------------------
static constexpr float WIN_W   = 1280.f;
static constexpr float WIN_H   =  720.f;
static constexpr float HUD_H   =   70.f;

static const sf::Color BAR_COLOR   { 12,  14,  34, 230 };
static const sf::Color BTN_NORMAL  { 45,  55, 100 };
static const sf::Color BTN_HOVER   { 80, 110, 190 };
static const sf::Color BTN_OUTLINE {100, 140, 220 };
static const sf::Color COIN_COLOR  {255, 215,   0 };
static const sf::Color COIN_RING   {218, 165,  32 };

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
HudBar::HudBar(AssetManager& assets)
{
    // ── Top bar ─────────────────────────────────────────────────────────────
    m_topBar.setSize({WIN_W, HUD_H});
    m_topBar.setPosition(0.f, 0.f);
    m_topBar.setFillColor(BAR_COLOR);

    // ── Bottom bar ───────────────────────────────────────────────────────────
    m_bottomBar.setSize({WIN_W, HUD_H});
    m_bottomBar.setPosition(0.f, WIN_H - HUD_H);
    m_bottomBar.setFillColor(BAR_COLOR);

    // ── Coin icon (circle) ───────────────────────────────────────────────────
    m_coinIcon.setRadius(20.f);
    m_coinIcon.setOrigin(20.f, 20.f);
    m_coinIcon.setFillColor(COIN_COLOR);
    m_coinIcon.setOutlineColor(COIN_RING);
    m_coinIcon.setOutlineThickness(3.f);
    m_coinIcon.setPosition(36.f, HUD_H * 0.5f);

    // ── Coin text ────────────────────────────────────────────────────────────
    m_coinText.setFont(assets.getFont("main"));
    m_coinText.setCharacterSize(26);
    m_coinText.setFillColor(sf::Color(255, 225, 60));
    m_coinText.setStyle(sf::Text::Bold);
    m_coinText.setOutlineColor(sf::Color(0, 0, 0, 180));
    m_coinText.setOutlineThickness(2.f);
    m_coinText.setPosition(68.f, HUD_H * 0.5f - 16.f);

    // ── Buttons ──────────────────────────────────────────────────────────────
    constexpr float BTN_W   = 250.f;
    constexpr float BTN_H   =  48.f;
    constexpr float BTN_Y   = WIN_H - HUD_H + (HUD_H - BTN_H) * 0.5f;
    constexpr float SPACING = (WIN_W - 2.f * BTN_W) / 3.f;

    struct BtnDef { const char* label; HudAction action; };
    static constexpr BtnDef DEFS[2] = {
        { "SHOP",          HudAction::Shop          },
        { "ENCYCLOPEDIA",  HudAction::Encyclopedia  },
    };

    for (int i = 0; i < 2; ++i) {
        float x = SPACING + static_cast<float>(i) * (BTN_W + SPACING);
        initButton(m_buttons[static_cast<std::size_t>(i)],
                   {x, BTN_Y, BTN_W, BTN_H},
                   DEFS[i].label, DEFS[i].action, assets);
    }
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
void HudBar::initButton(Button& btn, sf::FloatRect rect,
                        const std::string& label, HudAction action,
                        AssetManager& assets)
{
    btn.action      = action;
    btn.normalColor = BTN_NORMAL;
    btn.hoverColor  = BTN_HOVER;

    btn.shape.setSize({rect.width, rect.height});
    btn.shape.setPosition(rect.left, rect.top);
    btn.shape.setFillColor(BTN_NORMAL);
    btn.shape.setOutlineColor(BTN_OUTLINE);
    btn.shape.setOutlineThickness(2.f);

    btn.label.setFont(assets.getFont("main"));
    btn.label.setString(label);
    btn.label.setCharacterSize(18);
    btn.label.setFillColor(sf::Color::White);
    btn.label.setStyle(sf::Text::Bold);

    sf::FloatRect lb = btn.label.getLocalBounds();
    btn.label.setOrigin(lb.left + lb.width  * 0.5f,
                        lb.top  + lb.height * 0.5f);
    btn.label.setPosition(rect.left + rect.width  * 0.5f,
                          rect.top  + rect.height * 0.5f);
}

// ---------------------------------------------------------------------------
// Update
// ---------------------------------------------------------------------------
void HudBar::update(sf::Vector2f mousePos, int coins)
{
    m_coinText.setString(std::to_string(coins) + " coins");

    for (auto& btn : m_buttons) {
        bool over = btn.shape.getGlobalBounds().contains(mousePos);
        btn.hovered = over;
        btn.shape.setFillColor(over ? btn.hoverColor : btn.normalColor);
    }
}

// ---------------------------------------------------------------------------
// Draw
// ---------------------------------------------------------------------------
void HudBar::draw(sf::RenderWindow& window) const
{
    window.draw(m_topBar);
    window.draw(m_bottomBar);
    window.draw(m_coinIcon);
    window.draw(m_coinText);
    for (const auto& btn : m_buttons) {
        window.draw(btn.shape);
        window.draw(btn.label);
    }
}

// ---------------------------------------------------------------------------
// Input
// ---------------------------------------------------------------------------
HudAction HudBar::handleClick(sf::Vector2f point) const
{
    for (const auto& btn : m_buttons)
        if (btn.shape.getGlobalBounds().contains(point))
            return btn.action;
    return HudAction::None;
}
