#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

/// Lazy-loading texture / font cache.
/// Textures are generated procedurally when the corresponding file is absent.
class AssetManager {
public:
    AssetManager() = default;

    /// Returns a texture loaded from "assets/<key>.png".
    /// Falls back to a grey procedural circle if the file is absent.
    sf::Texture& getTexture(const std::string& key);

    /// Returns a font loaded from "assets/<key>.ttf"
    /// Falls back to common Windows / Linux system fonts.
    sf::Font& getFont(const std::string& key);

    /// Returns a distinct illustrated texture for the given pigeon tier (1-16).
    sf::Texture& getPigeonTexture(int tier);

    /// Returns the small brown circle used for visual poop FX.
    sf::Texture& getPoopTexture();

    /// Returns the gold coin circle used in the HUD.
    sf::Texture& getCoinTexture();

private:
    /// Draws a filled anti-aliased circle into an 80×80 image and caches it.
    sf::Texture& makeCircleTexture(const std::string& key,
                                   sf::Color fill,
                                   sf::Color outline = sf::Color::Transparent);

    /// Creates a procedural 1280×720 fallback background.
    sf::Texture& makeBackgroundTexture();

    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::Font>    m_fonts;
};
