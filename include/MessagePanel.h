#ifndef MESSAGEPANEL_H
#define MESSAGEPANEL_H

#include <SFML/Graphics.hpp>
#include "CustomString.h"

class MessagePanel
{
private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    CustomString m_message;

    bool m_visible;

public:
    MessagePanel();
    MessagePanel(const sf::Font& font, sf::Vector2f position, sf::Vector2f size, unsigned int characterSize = 18);

    void setFont(const sf::Font& font);
    void setMessage(const char* message);
    void clearMessage();
    void setVisible(bool visible);
    bool isVisible() const;

    void setPosition(sf::Vector2f position);
    void setSize(sf::Vector2f size);
    void setFillColor(const sf::Color& color);
    void setTextColor(const sf::Color& color);

    void draw(sf::RenderWindow& window);
};

#endif