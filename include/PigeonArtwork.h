#pragma once

#include <SFML/Graphics.hpp>

/// Renders one of the game's 16 pigeon characters as a transparent 2D image.
[[nodiscard]] sf::Image createPigeonArtwork(int tier);
