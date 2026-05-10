#include "MessagePanel.h"

MessagePanel::MessagePanel()
{
    m_visible = false;
    m_message.setText("");
}

MessagePanel::MessagePanel(const sf::Font& font, sf::Vector2f position, sf::Vector2f size, unsigned int characterSize)
{
    m_visible = false;
    m_message.setText("");

    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(sf::Color(45, 45, 45));
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color::White);

    m_text.setFont(font);
    m_text.setCharacterSize(characterSize);
    m_text.setFillColor(sf::Color::White);
    m_text.setPosition(position.x + 10.f, position.y + 6.f);
}

void MessagePanel::setFont(const sf::Font& font)
{
    m_text.setFont(font);
}

void MessagePanel::setMessage(const char* message)
{
    m_message.setText(message);
    m_text.setString(m_message.cStr());
    m_visible = true;
}

void MessagePanel::clearMessage()
{
    m_message.clear();
    m_text.setString("");
    m_visible = false;
}

void MessagePanel::setVisible(bool visible)
{
    m_visible = visible;
}

bool MessagePanel::isVisible() const
{
    return m_visible;
}

void MessagePanel::setPosition(sf::Vector2f position)
{
    m_shape.setPosition(position);
    m_text.setPosition(position.x + 10.f, position.y + 6.f);
}

void MessagePanel::setSize(sf::Vector2f size)
{
    m_shape.setSize(size);
}

void MessagePanel::setFillColor(const sf::Color& color)
{
    m_shape.setFillColor(color);
}

void MessagePanel::setTextColor(const sf::Color& color)
{
    m_text.setFillColor(color);
}

void MessagePanel::draw(sf::RenderWindow& window)
{
    if (!m_visible)
    {
        return;
    }

    window.draw(m_shape);
    window.draw(m_text);
}