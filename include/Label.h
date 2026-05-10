#ifndef LABEL_H
#define LABEL_H

#include <SFML/Graphics.hpp>
#include "CustomString.h"

class Label
{
private:
    sf::Text m_text;
    CustomString m_content;

public:
    Label();
    Label(const sf::Font& font, const char* content, unsigned int characterSize, sf::Vector2f position);

    void setFont(const sf::Font& font);
    void setText(const char* content);
    void setCharacterSize(unsigned int characterSize);
    void setPosition(sf::Vector2f position);
    void setFillColor(const sf::Color& color);

    const CustomString& getText() const;
    sf::Vector2f getPosition() const;

    void draw(sf::RenderWindow& window);
};

#endif