#include "AssetManager.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <string>

// ---------------------------------------------------------------------------
// Tier colour palette (indices 0-15 map to pigeon tiers 1-16)
// ---------------------------------------------------------------------------
static const std::array<sf::Color, 16> TIER_COLORS = {{
    {135, 206, 250},  // 1  baby blue
    { 70, 130, 180},  // 2  steel blue
    { 60, 179, 113},  // 3  sea green
    {154, 205,  50},  // 4  yellow-green
    {255, 215,   0},  // 5  gold
    {255, 140,   0},  // 6  dark orange
    {255,  69,   0},  // 7  orange-red
    {220,  20,  60},  // 8  crimson
    {199,  21, 133},  // 9  medium violet-red
    {128,   0, 128},  // 10 purple
    { 75,   0, 130},  // 11 indigo
    {148,   0, 211},  // 12 dark violet
    {  0, 128, 128},  // 13 teal
    {  0, 206, 209},  // 14 dark turquoise
    {212, 175,  55},  // 15 metallic gold
    {255, 255, 255},  // 16 legendary white
}};

// ---------------------------------------------------------------------------
// Procedural circle texture (80×80, with subtle radial gradient + outline)
// ---------------------------------------------------------------------------
sf::Texture& AssetManager::makeCircleTexture(const std::string& key,
                                              sf::Color          fill,
                                              sf::Color          outline)
{
    constexpr int SIZE  = 80;
    const float   cx    = SIZE / 2.f;
    const float   cy    = SIZE / 2.f;
    const float   outerR = SIZE / 2.f - 1.f;
    const float   innerR = outerR - (outline != sf::Color::Transparent ? 4.f : 0.f);

    sf::Image img;
    img.create(SIZE, SIZE, sf::Color::Transparent);

    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            float dx = static_cast<float>(x) - cx;
            float dy = static_cast<float>(y) - cy;
            float r  = std::sqrt(dx * dx + dy * dy);

            if (r <= innerR) {
                // Radial brightness gradient: brighter at centre
                float t  = 1.f - r / innerR;
                sf::Color c = fill;
                c.r = static_cast<sf::Uint8>(std::min(255, static_cast<int>(c.r + t * 55)));
                c.g = static_cast<sf::Uint8>(std::min(255, static_cast<int>(c.g + t * 55)));
                c.b = static_cast<sf::Uint8>(std::min(255, static_cast<int>(c.b + t * 55)));
                img.setPixel(x, y, c);
            } else if (r <= outerR && outline != sf::Color::Transparent) {
                img.setPixel(x, y, outline);
            }
        }
    }

    m_textures[key].loadFromImage(img);
    m_textures[key].setSmooth(true);
    return m_textures[key];
}

// ---------------------------------------------------------------------------
// Procedural 1280×720 dark gradient background with a star field
// ---------------------------------------------------------------------------
sf::Texture& AssetManager::makeBackgroundTexture()
{
    const std::string key = "background";
    constexpr int W = 1280, H = 720;

    sf::Image img;
    img.create(W, H, sf::Color::Black);

    // Vertical gradient: deep space top → slightly lighter near the ground
    for (int y = 0; y < H; ++y) {
        float t = static_cast<float>(y) / H;
        sf::Uint8 r = static_cast<sf::Uint8>( 8 + t *  8);
        sf::Uint8 g = static_cast<sf::Uint8>( 8 + t * 12);
        sf::Uint8 b = static_cast<sf::Uint8>(20 + t * 30);
        for (int x = 0; x < W; ++x)
            img.setPixel(x, y, sf::Color(r, g, b));
    }

    // Deterministic star field (LCG – does not touch global rand state)
    auto lcg = [](unsigned& s) -> unsigned { return s = s * 1664525u + 1013904223u; };
    unsigned state = 0xDEADBEEFu;
    for (int i = 0; i < 220; ++i) {
        int sx = static_cast<int>(lcg(state) % W);
        int sy = static_cast<int>(lcg(state) % static_cast<unsigned>(H * 2 / 3));
        sf::Uint8 br = static_cast<sf::Uint8>(140 + lcg(state) % 116);
        img.setPixel(sx, sy, sf::Color(br, br, br));
    }

    // Silhouette rooftops at the bottom strip
    for (int x = 0; x < W; ++x) {
        // Simple sawtooth silhouette
        int buildingH = 18 + ((x / 80) % 4) * 12 + ((x / 40) % 3) * 6;
        for (int y = H - buildingH; y < H; ++y)
            img.setPixel(x, y, sf::Color(14, 14, 26));
    }

    m_textures[key].loadFromImage(img);
    m_textures[key].setSmooth(false);
    return m_textures[key];
}

// ---------------------------------------------------------------------------
// Public interface
// ---------------------------------------------------------------------------
sf::Texture& AssetManager::getTexture(const std::string& key)
{
    auto it = m_textures.find(key);
    if (it != m_textures.end()) return it->second;

    // Try loading from file
    std::string path = "assets/" + key + ".png";
    if (std::filesystem::exists(path)) {
        if (m_textures[key].loadFromFile(path)) {
            m_textures[key].setSmooth(true);
            return m_textures[key];
        }
    }

    // Special procedural fallbacks
    if (key == "background") return makeBackgroundTexture();

    return makeCircleTexture(key, sf::Color(120, 120, 120));
}

sf::Font& AssetManager::getFont(const std::string& key)
{
    auto it = m_fonts.find(key);
    if (it != m_fonts.end()) return it->second;

    const std::vector<std::string> candidates = {
        "assets/" + key + ".ttf",
        "assets/" + key + ".otf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/calibri.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
    };
    for (const auto& p : candidates)
        if (m_fonts[key].loadFromFile(p)) return m_fonts[key];

    return m_fonts[key]; // empty font – SFML will warn
}

sf::Texture& AssetManager::getPigeonTexture(int tier)
{
    std::string key = "pigeon_t" + std::to_string(tier);
    auto it = m_textures.find(key);
    if (it != m_textures.end()) return it->second;

    std::string path = "assets/" + key + ".png";
    if (std::filesystem::exists(path)) {
        if (m_textures[key].loadFromFile(path)) {
            m_textures[key].setSmooth(true);
            return m_textures[key];
        }
    }

    int idx = std::clamp(tier - 1, 0, 15);
    sf::Color fill    = TIER_COLORS[static_cast<std::size_t>(idx)];
    sf::Color outline = (tier == 16) ? sf::Color(255, 215, 0)
                                     : sf::Color(255, 255, 255, 160);
    return makeCircleTexture(key, fill, outline);
}

sf::Texture& AssetManager::getPoopTexture()
{
    constexpr auto KEY = "poop";
    auto it = m_textures.find(KEY);
    if (it != m_textures.end()) return it->second;
    return makeCircleTexture(KEY, sf::Color(101, 67, 33));
}

sf::Texture& AssetManager::getCoinTexture()
{
    constexpr auto KEY = "coin";
    auto it = m_textures.find(KEY);
    if (it != m_textures.end()) return it->second;
    return makeCircleTexture(KEY, sf::Color(255, 215, 0), sf::Color(218, 165, 32));
}
