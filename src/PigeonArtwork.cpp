#include "PigeonArtwork.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <initializer_list>

namespace {
constexpr unsigned IMAGE_SIZE = 80;
const sf::Color INK(43, 48, 59);
const sf::Color BEAK(248, 174, 73);
const sf::Color FEET(228, 157, 64);

const std::array<sf::Color, 16> BODY_COLORS = {{
    {164, 197, 220}, {105, 137, 163}, {111, 153, 136}, {144, 143, 132},
    {168, 150, 141}, {132, 165, 120}, {141, 137, 160}, {114, 145, 170},
    {153, 151, 190}, {150, 153, 135}, {132, 139, 166}, {112, 143, 151},
    {130, 143, 159}, {145, 151, 134}, {168, 154, 139}, {188, 190, 191}
}};

sf::Color darker(const sf::Color color, const float amount = 0.72f)
{
    return {
        static_cast<sf::Uint8>(color.r * amount),
        static_cast<sf::Uint8>(color.g * amount),
        static_cast<sf::Uint8>(color.b * amount)
    };
}

sf::Color lighter(const sf::Color color, const float amount = 0.35f)
{
    return {
        static_cast<sf::Uint8>(std::min(255.f, color.r + (255.f - color.r) * amount)),
        static_cast<sf::Uint8>(std::min(255.f, color.g + (255.f - color.g) * amount)),
        static_cast<sf::Uint8>(std::min(255.f, color.b + (255.f - color.b) * amount))
    };
}

void ellipse(sf::RenderTarget& target, const sf::Vector2f center,
             const sf::Vector2f radius, const sf::Color fill,
             const sf::Color outline = INK, const float outlineWidth = 1.5f)
{
    sf::CircleShape shape(radius.x, 48);
    shape.setOrigin(radius.x, radius.x);
    shape.setPosition(center);
    shape.setScale(1.f, radius.y / radius.x);
    shape.setFillColor(fill);
    shape.setOutlineColor(outline);
    shape.setOutlineThickness(outlineWidth);
    target.draw(shape);
}

void polygon(sf::RenderTarget& target,
             const std::initializer_list<sf::Vector2f> points,
             const sf::Color fill, const sf::Color outline = INK,
             const float outlineWidth = 1.5f)
{
    sf::ConvexShape shape(points.size());
    std::size_t index = 0;
    for (const auto point : points)
        shape.setPoint(index++, point);
    shape.setFillColor(fill);
    shape.setOutlineColor(outline);
    shape.setOutlineThickness(outlineWidth);
    target.draw(shape);
}

void segment(sf::RenderTarget& target, const sf::Vector2f from,
             const sf::Vector2f to, const sf::Color color, const float thickness)
{
    const sf::Vector2f delta = to - from;
    sf::RectangleShape shape({std::sqrt(delta.x * delta.x + delta.y * delta.y), thickness});
    shape.setOrigin(0.f, thickness * 0.5f);
    shape.setPosition(from);
    shape.setRotation(std::atan2(delta.y, delta.x) * 180.f / 3.14159265f);
    shape.setFillColor(color);
    target.draw(shape);
}

void eye(sf::RenderTarget& target, const sf::Vector2f center, const float radius = 2.4f)
{
    ellipse(target, center, {radius, radius}, INK, INK, 0.f);
    ellipse(target, {center.x + 0.7f, center.y - 0.8f}, {0.7f, 0.7f},
            sf::Color::White, sf::Color::White, 0.f);
}

void star(sf::RenderTarget& target, const sf::Vector2f center, const float radius,
          const sf::Color color)
{
    polygon(target, {{center.x, center.y}, {center.x - radius * 0.24f, center.y - radius * 0.2f},
                     {center.x, center.y - radius}}, color, INK, 0.7f);
    polygon(target, {{center.x, center.y}, {center.x + radius * 0.24f, center.y - radius * 0.2f},
                     {center.x + radius, center.y}}, color, INK, 0.7f);
    polygon(target, {{center.x, center.y}, {center.x + radius * 0.2f, center.y + radius * 0.24f},
                     {center.x, center.y + radius}}, color, INK, 0.7f);
    polygon(target, {{center.x, center.y}, {center.x - radius * 0.2f, center.y + radius * 0.24f},
                     {center.x - radius, center.y}}, color, INK, 0.7f);
}

void drawFeet(sf::RenderTarget& target, const float ground, const bool longLegs)
{
    const float footY = longLegs ? ground + 2.f : ground;
    const float legStart = longLegs ? ground - 11.f : ground - 5.f;
    segment(target, {34.f, legStart}, {34.f, footY}, FEET, 2.2f);
    segment(target, {47.f, legStart}, {47.f, footY}, FEET, 2.2f);
    for (const float x : {34.f, 47.f}) {
        segment(target, {x, footY}, {x - 4.f, footY + 2.f}, FEET, 1.8f);
        segment(target, {x, footY}, {x + 4.f, footY + 2.f}, FEET, 1.8f);
    }
}

void drawFeatherTuft(sf::RenderTarget& target, const sf::Color color)
{
    polygon(target, {{39.f, 20.f}, {39.f, 12.f}, {44.f, 19.f}}, color, INK, 1.1f);
    polygon(target, {{43.f, 20.f}, {47.f, 11.f}, {50.f, 20.f}}, color, INK, 1.1f);
    polygon(target, {{47.f, 20.f}, {54.f, 15.f}, {51.f, 22.f}}, color, INK, 1.1f);
}

void drawOrdinaryPigeon(sf::RenderTarget& target, const int tier,
                        const sf::Color bodyColor)
{
    const sf::Color wingColor = darker(bodyColor, tier == 16 ? 0.58f : 0.76f);
    const bool tinyBaby = tier == 1;
    const bool elongatedWorm = tier == 7;
    const bool springPigeon = tier == 8;
    const bool twinPigeon = tier == 11;
    const bool ostrichPigeon = tier == 16;

    float bodyX = 21.f;
    float bodyY = 16.f;
    float bodyCenterY = springPigeon ? 36.f : 48.f;
    float headX = 48.f;
    float headY = 28.f;
    float headRadius = 9.f;
    if (tinyBaby) { bodyX = 16.f; bodyY = 13.f; bodyCenterY = 50.f; headX = 46.f; headY = 34.f; headRadius = 11.f; }
    if (tier == 3) { bodyX = 23.f; bodyY = 18.f; }
    if (tier == 4) { bodyX = 25.f; bodyY = 19.f; }
    if (tier == 5) { bodyX = 26.f; bodyY = 21.f; bodyCenterY = 49.f; }
    if (tier == 9) { bodyX = 22.f; bodyY = 17.f; }
    if (tier == 10) { bodyX = 23.f; bodyY = 17.f; }
    if (tier == 12) { headY = 29.f; }
    if (tier == 15) { bodyX = 27.f; bodyY = 22.f; bodyCenterY = 48.f; headRadius = 8.f; }
    if (ostrichPigeon) { bodyX = 20.f; bodyY = 14.f; bodyCenterY = 53.f; headX = 47.f; headY = 14.f; headRadius = 7.f; }

    // The worm form is a row of soft feathered segments instead of one body.
    if (elongatedWorm) {
        ellipse(target, {24.f, 48.f}, {13.f, 12.f}, bodyColor, INK, 1.5f);
        ellipse(target, {39.f, 49.f}, {13.f, 11.f}, lighter(bodyColor, 0.12f), INK, 1.5f);
        ellipse(target, {53.f, 48.f}, {11.f, 9.f}, darker(bodyColor, 0.12f), INK, 1.5f);
    } else {
        // Ostrich-like tiers get a noticeably longer neck.
        if (ostrichPigeon)
            ellipse(target, {43.f, 33.f}, {6.f, 17.f}, bodyColor, INK, 1.5f);
        else if (!tinyBaby)
            ellipse(target, {43.f, bodyCenterY - 12.f}, {8.f, 12.f}, bodyColor, INK, 1.5f);
        ellipse(target, {39.f, bodyCenterY}, {bodyX, bodyY}, bodyColor, INK, 1.8f);
    }

    const float tailX = elongatedWorm ? 10.f : 17.f;
    polygon(target, {{tailX + 13.f, bodyCenterY - 6.f}, {tailX, bodyCenterY - 8.f},
                     {tailX + 3.f, bodyCenterY + 8.f}, {tailX + 15.f, bodyCenterY + 7.f}},
            wingColor, INK, 1.3f);

    if (springPigeon) {
        // A spring replaces the usual feet and makes the tier read as bouncy.
        segment(target, {39.f, 54.f}, {34.f, 58.f}, sf::Color(88, 96, 115), 2.f);
        segment(target, {34.f, 58.f}, {44.f, 62.f}, sf::Color(88, 96, 115), 2.f);
        segment(target, {44.f, 62.f}, {34.f, 66.f}, sf::Color(88, 96, 115), 2.f);
        segment(target, {34.f, 66.f}, {44.f, 70.f}, sf::Color(88, 96, 115), 2.f);
        segment(target, {44.f, 70.f}, {39.f, 73.f}, sf::Color(88, 96, 115), 2.f);
        segment(target, {31.f, 74.f}, {47.f, 74.f}, INK, 2.f);
    } else {
        const float feetGround = std::min(76.f, bodyCenterY + bodyY + 7.f);
        drawFeet(target, ostrichPigeon ? 72.f : feetGround, ostrichPigeon);
    }

    // Breast and layered wing provide the recognizable pigeon silhouette.
    const sf::Color breastColor = lighter(bodyColor, 0.45f);
    ellipse(target, {46.f, bodyCenterY + 5.f}, {10.f, 11.f}, breastColor,
            sf::Color::Transparent, 0.f);
    ellipse(target, {33.f, bodyCenterY - 1.f},
            {tinyBaby ? 11.f : (tier == 15 ? 13.f : 15.f), tinyBaby ? 7.f : 9.f},
            wingColor, INK, 1.3f);
    for (int feather = 0; feather < 3; ++feather) {
        const float x = 26.f + static_cast<float>(feather) * 6.f;
        const float y = bodyCenterY - 2.f + static_cast<float>(feather) * 2.f;
        ellipse(target, {x, y}, {4.f, 2.1f}, lighter(wingColor, 0.2f),
                sf::Color::Transparent, 0.f);
    }

    if (tier == 15) {
        // Bellybird's defining round, pale belly sits over its tiny wings.
        ellipse(target, {43.f, 50.f}, {17.f, 16.f}, sf::Color(232, 216, 185), INK, 1.2f);
        ellipse(target, {33.f, 47.f}, {8.f, 5.f}, wingColor, INK, 1.f);
    }

    if (twinPigeon) {
        ellipse(target, {34.f, 28.f}, {7.f, 7.f}, lighter(bodyColor), INK, 1.3f);
        eye(target, {36.f, 27.f}, 1.8f);
        polygon(target, {{28.f, 29.f}, {23.f, 31.f}, {29.f, 33.f}}, BEAK, INK, 0.8f);
    }

    ellipse(target, {headX, headY}, {headRadius, headRadius},
            lighter(bodyColor, 0.12f), INK, 1.6f);
    polygon(target, {{headX + headRadius - 2.f, headY - 1.f},
                     {headX + headRadius + 6.f, headY + 1.f},
                     {headX + headRadius - 2.f, headY + 3.5f}}, BEAK, INK, 1.f);
    eye(target, {headX + 2.3f, headY - 1.5f}, tinyBaby ? 2.7f : 2.4f);
    ellipse(target, {headX + 5.f, headY + 5.f}, {2.2f, 1.3f},
            sf::Color(232, 139, 139), sf::Color::Transparent, 0.f);

    if (tinyBaby) {
        drawFeatherTuft(target, lighter(bodyColor, 0.48f));
        ellipse(target, {39.f, 43.f}, {3.f, 2.f}, sf::Color(250, 233, 195),
                sf::Color::Transparent, 0.f);
    }
    if (tier == 3) {
        for (const sf::Vector2f crumb : {sf::Vector2f{25.f, 58.f}, {30.f, 61.f}, {35.f, 60.f}})
            ellipse(target, crumb, {1.2f, 1.2f}, sf::Color(230, 196, 135),
                    sf::Color::Transparent, 0.f);
    }
    if (tier == 4 || tier == 5) {
        segment(target, {26.f, 52.f}, {53.f, 52.f}, sf::Color(118, 71, 83), 2.3f);
        ellipse(target, {40.f, 53.f}, {3.f, 3.f}, sf::Color(236, 180, 111), INK, 0.8f);
    }
    if (tier == 6) {
        drawFeatherTuft(target, sf::Color(126, 193, 119));
        eye(target, {43.f, 23.f}, 1.8f);
        ellipse(target, {25.f, 39.f}, {3.f, 2.f}, sf::Color(126, 193, 119), INK, 0.7f);
    }
    if (elongatedWorm) {
        segment(target, {10.f, 39.f}, {8.f, 31.f}, sf::Color(107, 76, 118), 1.5f);
        ellipse(target, {8.f, 30.f}, {2.f, 2.f}, sf::Color(241, 202, 126), INK, 0.6f);
        for (const float x : {24.f, 39.f, 53.f})
            segment(target, {x, 56.f}, {x - 1.f, 62.f}, FEET, 1.4f);
    }
    if (tier == 9) {
        ellipse(target, {20.f, 26.f}, {4.f, 4.f}, sf::Color(255, 132, 171), INK, 0.9f);
        ellipse(target, {24.f, 22.f}, {4.f, 4.f}, sf::Color(255, 215, 91), INK, 0.9f);
        ellipse(target, {28.f, 26.f}, {4.f, 4.f}, sf::Color(111, 191, 235), INK, 0.9f);
        star(target, {57.f, 18.f}, 4.f, sf::Color(255, 219, 94));
    }
    if (tier == 10) {
        polygon(target, {{39.f, 21.f}, {39.f, 14.f}, {45.f, 21.f}},
                sf::Color(214, 75, 72), INK, 1.1f);
        polygon(target, {{43.f, 21.f}, {47.f, 10.f}, {51.f, 21.f}},
                sf::Color(214, 75, 72), INK, 1.1f);
        polygon(target, {{48.f, 21.f}, {55.f, 15.f}, {52.f, 23.f}},
                sf::Color(214, 75, 72), INK, 1.1f);
        ellipse(target, {57.f, 33.f}, {2.3f, 3.3f}, sf::Color(201, 68, 66), INK, 0.8f);
        polygon(target, {{24.f, 44.f}, {12.f, 39.f}, {20.f, 49.f}},
                sf::Color(221, 139, 78), INK, 1.f);
        polygon(target, {{20.f, 49.f}, {12.f, 55.f}, {27.f, 55.f}},
                sf::Color(221, 139, 78), INK, 1.f);
    }
    if (tier == 12) {
        // Round spectacles and a tiny scholar's cap.
        ellipse(target, {49.f, 28.f}, {4.f, 3.6f}, sf::Color::Transparent,
                sf::Color(64, 73, 94), 1.2f);
        segment(target, {44.f, 28.f}, {45.f, 28.f}, sf::Color(64, 73, 94), 1.f);
        polygon(target, {{38.f, 19.f}, {46.f, 13.f}, {55.f, 19.f}, {47.f, 22.f}},
                sf::Color(74, 70, 105), INK, 1.f);
        segment(target, {47.f, 17.f}, {58.f, 17.f}, sf::Color(236, 194, 91), 1.2f);
        ellipse(target, {58.f, 18.f}, {1.5f, 1.5f}, sf::Color(236, 194, 91), INK, 0.5f);
    }
    if (tier == 13) {
        for (const sf::Vector2f position : {sf::Vector2f{40.f, 18.f}, {33.f, 35.f},
                                             {41.f, 38.f}, {48.f, 39.f}})
            eye(target, position, 1.8f);
    }
    if (tier == 14) {
        // Collector badge and a little net make this the bird that catches things.
        ellipse(target, {34.f, 48.f}, {4.f, 4.f}, sf::Color(239, 192, 85), INK, 0.8f);
        star(target, {34.f, 48.f}, 2.5f, sf::Color(255, 246, 190));
        segment(target, {57.f, 27.f}, {68.f, 14.f}, sf::Color(104, 74, 54), 2.f);
        ellipse(target, {68.f, 13.f}, {6.f, 7.f}, sf::Color(170, 203, 218),
                sf::Color(74, 103, 126), 1.3f);
        segment(target, {64.f, 9.f}, {71.f, 17.f}, sf::Color(74, 103, 126), 0.8f);
        segment(target, {71.f, 9.f}, {64.f, 17.f}, sf::Color(74, 103, 126), 0.8f);
    }
    if (ostrichPigeon) {
        drawFeatherTuft(target, sf::Color(222, 143, 91));
        ellipse(target, {36.f, 70.f}, {4.f, 2.f}, sf::Color(118, 92, 73), INK, 0.7f);
    }
}

} // namespace

sf::Image createPigeonArtwork(const int tier)
{
    const int safeTier = std::clamp(tier, 1, 16);
    const sf::Color bodyColor = BODY_COLORS[static_cast<std::size_t>(safeTier - 1)];

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderTexture canvas;
    if (!canvas.create(IMAGE_SIZE, IMAGE_SIZE, settings))
        return {};

    canvas.clear(sf::Color::Transparent);
    drawOrdinaryPigeon(canvas, safeTier, bodyColor);
    canvas.display();

    sf::Image image = canvas.getTexture().copyToImage();
    return image;
}
