#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <SFML/Graphics.hpp>
#include "CustomString.h"

class InputBox
{
private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    sf::Text m_placeholderText;

    CustomString m_textContent;
    CustomString m_placeholder;

    bool m_isFocused;
    bool m_isPassword;
    unsigned int m_maxLength;

public:
    InputBox();
    InputBox(
        const sf::Font& font,
        const char* placeholder,
        sf::Vector2f position,
        sf::Vector2f size,
        unsigned int characterSize = 20,
        bool isPassword = false,
        unsigned int maxLength = 100
    );

    void setFont(const sf::Font& font);
    void setPlaceholder(const char* placeholder);
    void setPosition(sf::Vector2f position);
    void setSize(sf::Vector2f size);
    void setFocused(bool focused);
    void setPasswordMode(bool isPassword);
    void setMaxLength(unsigned int maxLength);

    void clear();
    void setText(const char* text);
    const CustomString& getText() const;
    bool isFocused() const;

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update();

    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
};

#endif