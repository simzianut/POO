#pragma once

#include <algorithm>
#include <SFML/Graphics.hpp>

class OverlayScrollBar {
public:
    void configure(const sf::FloatRect trackBounds, const float contentHeight,
                   const float viewportHeight)
    {
        m_trackBounds = trackBounds;
        m_viewportHeight = std::max(0.f, viewportHeight);
        m_maxOffset = std::max(0.f, contentHeight - m_viewportHeight);
        m_offset = std::clamp(m_offset, 0.f, m_maxOffset);
        m_scrollable = m_maxOffset > 0.f && m_trackBounds.height > 0.f;

        m_track.setPosition(m_trackBounds.left, m_trackBounds.top);
        m_track.setSize({m_trackBounds.width, m_trackBounds.height});
        m_track.setFillColor(sf::Color(7, 9, 20, 190));

        if (!m_scrollable) {
            m_thumb.setSize({m_trackBounds.width, m_trackBounds.height});
            m_thumb.setPosition(m_trackBounds.left, m_trackBounds.top);
            return;
        }

        const float thumbHeight = std::min(
            m_trackBounds.height,
            std::max(32.f, m_trackBounds.height * m_viewportHeight / contentHeight));
        m_thumb.setSize({m_trackBounds.width, thumbHeight});
        m_thumb.setFillColor(sf::Color(115, 130, 190, 230));
        updateThumbPosition();
    }

    [[nodiscard]] float offset() const { return m_offset; }
    [[nodiscard]] bool isScrollable() const { return m_scrollable; }

    bool handlePress(const sf::Vector2f position)
    {
        if (!m_scrollable || !m_trackBounds.contains(position))
            return false;

        const sf::FloatRect thumbBounds = m_thumb.getGlobalBounds();
        m_dragging = true;
        if (thumbBounds.contains(position)) {
            m_dragGrabOffset = position.y - thumbBounds.top;
        } else {
            m_dragGrabOffset = thumbBounds.height * 0.5f;
            moveThumbTo(position.y - m_dragGrabOffset);
        }
        return true;
    }

    bool handleMove(const sf::Vector2f position)
    {
        if (!m_dragging)
            return false;
        moveThumbTo(position.y - m_dragGrabOffset);
        return true;
    }

    void handleRelease() { m_dragging = false; }

    bool handleWheel(const sf::Vector2f position, const float delta,
                     const sf::FloatRect contentBounds)
    {
        if (!m_scrollable || (!contentBounds.contains(position) && !m_trackBounds.contains(position)))
            return false;

        m_offset = std::clamp(m_offset - delta * 42.f, 0.f, m_maxOffset);
        updateThumbPosition();
        return true;
    }

    void reset()
    {
        m_offset = 0.f;
        m_dragging = false;
        updateThumbPosition();
    }

    void draw(sf::RenderTarget& target) const
    {
        if (!m_scrollable)
            return;
        target.draw(m_track);
        target.draw(m_thumb);
    }

private:
    void updateThumbPosition()
    {
        const float thumbTravel = std::max(0.f, m_trackBounds.height - m_thumb.getSize().y);
        const float ratio = m_maxOffset > 0.f ? m_offset / m_maxOffset : 0.f;
        m_thumb.setPosition(m_trackBounds.left, m_trackBounds.top + thumbTravel * ratio);
    }

    void moveThumbTo(const float top)
    {
        const float thumbTravel = std::max(0.f, m_trackBounds.height - m_thumb.getSize().y);
        const float clampedTop = std::clamp(top, m_trackBounds.top, m_trackBounds.top + thumbTravel);
        const float ratio = thumbTravel > 0.f ? (clampedTop - m_trackBounds.top) / thumbTravel : 0.f;
        m_offset = ratio * m_maxOffset;
        m_thumb.setPosition(m_trackBounds.left, clampedTop);
    }

    sf::FloatRect m_trackBounds{};
    sf::RectangleShape m_track;
    sf::RectangleShape m_thumb;
    float m_viewportHeight = 0.f;
    float m_maxOffset = 0.f;
    float m_offset = 0.f;
    float m_dragGrabOffset = 0.f;
    bool m_scrollable = false;
    bool m_dragging = false;
};
