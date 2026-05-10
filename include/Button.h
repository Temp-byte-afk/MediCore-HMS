#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include "CustomString.h"

class Button
{
private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    CustomString m_label;

    bool m_hovered;
    bool m_pressed;

public:
    Button();
    Button(const sf::Font& font, const char* label, sf::Vector2f position, sf::Vector2f size, unsigned int characterSize = 20);

    void setFont(const sf::Font& font);
    void setLabel(const char* label);
    void setPosition(sf::Vector2f position);
    void setSize(sf::Vector2f size);
    void setFillColor(const sf::Color& color);
    void setOutlineColor(const sf::Color& color);
    void setTextColor(const sf::Color& color);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    bool isHovered(const sf::RenderWindow& window) const;
    bool isClicked(const sf::Event& event, const sf::RenderWindow& window) const;

    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
};

#endif