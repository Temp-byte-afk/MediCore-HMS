#include "Button.h"

Button::Button()
{
    m_hovered = false;
    m_pressed = false;
    m_label.setText("");
}

Button::Button(const sf::Font& font, const char* label, sf::Vector2f position, sf::Vector2f size, unsigned int characterSize)
{
    m_hovered = false;
    m_pressed = false;
    m_label.setText(label);

    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(sf::Color(60, 60, 60));
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color::White);

    m_text.setFont(font);
    m_text.setString(m_label.cStr());
    m_text.setCharacterSize(characterSize);
    m_text.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_text.setPosition(position.x + size.x / 2.f, position.y + size.y / 2.f - 4.f);
}

void Button::setFont(const sf::Font& font)
{
    m_text.setFont(font);
}

void Button::setLabel(const char* label)
{
    m_label.setText(label);
    m_text.setString(m_label.cStr());

    sf::FloatRect textBounds = m_text.getLocalBounds();
    sf::Vector2f pos = m_shape.getPosition();
    sf::Vector2f size = m_shape.getSize();
    m_text.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_text.setPosition(pos.x + size.x / 2.f, pos.y + size.y / 2.f - 4.f);
}

void Button::setPosition(sf::Vector2f position)
{
    m_shape.setPosition(position);

    sf::FloatRect textBounds = m_text.getLocalBounds();
    sf::Vector2f size = m_shape.getSize();
    m_text.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_text.setPosition(position.x + size.x / 2.f, position.y + size.y / 2.f - 4.f);
}

void Button::setSize(sf::Vector2f size)
{
    m_shape.setSize(size);

    sf::FloatRect textBounds = m_text.getLocalBounds();
    sf::Vector2f pos = m_shape.getPosition();
    m_text.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_text.setPosition(pos.x + size.x / 2.f, pos.y + size.y / 2.f - 4.f);
}

void Button::setFillColor(const sf::Color& color)
{
    m_shape.setFillColor(color);
}

void Button::setOutlineColor(const sf::Color& color)
{
    m_shape.setOutlineColor(color);
}

void Button::setTextColor(const sf::Color& color)
{
    m_text.setFillColor(color);
}

bool Button::isHovered(const sf::RenderWindow& window) const
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return m_shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

bool Button::isClicked(const sf::Event& event, const sf::RenderWindow& window) const
{
    if (event.type != sf::Event::MouseButtonPressed)
    {
        return false;
    }

    if (event.mouseButton.button != sf::Mouse::Left)
    {
        return false;
    }

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return m_shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_hovered = isHovered(window);

    if (m_hovered)
    {
        m_shape.setFillColor(sf::Color(90, 90, 90));
    }
    else
    {
        m_shape.setFillColor(sf::Color(60, 60, 60));
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        m_pressed = isClicked(event, window);
    }
    else
    {
        m_pressed = false;
    }
}

void Button::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
    window.draw(m_text);
}

sf::FloatRect Button::getBounds() const
{
    return m_shape.getGlobalBounds();
}