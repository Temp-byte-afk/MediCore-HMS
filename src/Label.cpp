#include "Label.h"

Label::Label()
{
    m_content.setText("");
}

Label::Label(const sf::Font& font, const char* content, unsigned int characterSize, sf::Vector2f position)
{
    m_content.setText(content);
    m_text.setFont(font);
    m_text.setString(m_content.cStr());
    m_text.setCharacterSize(characterSize);
    m_text.setPosition(position);
}

void Label::setFont(const sf::Font& font)
{
    m_text.setFont(font);
}

void Label::setText(const char* content)
{
    m_content.setText(content);
    m_text.setString(m_content.cStr());
}

void Label::setCharacterSize(unsigned int characterSize)
{
    m_text.setCharacterSize(characterSize);
}

void Label::setPosition(sf::Vector2f position)
{
    m_text.setPosition(position);
}

void Label::setFillColor(const sf::Color& color)
{
    m_text.setFillColor(color);
}

const CustomString& Label::getText() const
{
    return m_content;
}

sf::Vector2f Label::getPosition() const
{
    return m_text.getPosition();
}

void Label::draw(sf::RenderWindow& window)
{
    window.draw(m_text);
}