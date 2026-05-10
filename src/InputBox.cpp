#include "InputBox.h"

InputBox::InputBox()
{
    m_isFocused = false;
    m_isPassword = false;
    m_maxLength = 100;
    m_textContent.setText("");
    m_placeholder.setText("");
}

InputBox::InputBox(
    const sf::Font& font,
    const char* placeholder,
    sf::Vector2f position,
    sf::Vector2f size,
    unsigned int characterSize,
    bool isPassword,
    unsigned int maxLength)
{
    m_isFocused = false;
    m_isPassword = isPassword;
    m_maxLength = maxLength;
    m_textContent.setText("");
    m_placeholder.setText(placeholder);

    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(sf::Color(35, 35, 35));
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color::White);

    m_text.setFont(font);
    m_text.setCharacterSize(characterSize);
    m_text.setFillColor(sf::Color::White);
    m_text.setPosition(position.x + 10.f, position.y + 5.f);

    m_placeholderText.setFont(font);
    m_placeholderText.setString(m_placeholder.cStr());
    m_placeholderText.setCharacterSize(characterSize);
    m_placeholderText.setFillColor(sf::Color(160, 160, 160));
    m_placeholderText.setPosition(position.x + 10.f, position.y + 5.f);
}

void InputBox::setFont(const sf::Font& font)
{
    m_text.setFont(font);
    m_placeholderText.setFont(font);
}

void InputBox::setPlaceholder(const char* placeholder)
{
    m_placeholder.setText(placeholder);
    m_placeholderText.setString(m_placeholder.cStr());
}

void InputBox::setPosition(sf::Vector2f position)
{
    m_shape.setPosition(position);
    m_text.setPosition(position.x + 10.f, position.y + 5.f);
    m_placeholderText.setPosition(position.x + 10.f, position.y + 5.f);
}

void InputBox::setSize(sf::Vector2f size)
{
    m_shape.setSize(size);
}

void InputBox::setFocused(bool focused)
{
    m_isFocused = focused;
}

void InputBox::setPasswordMode(bool isPassword)
{
    m_isPassword = isPassword;
}

void InputBox::setMaxLength(unsigned int maxLength)
{
    m_maxLength = maxLength;
}

void InputBox::clear()
{
    m_textContent.clear();
    m_text.setString("");
}

void InputBox::setText(const char* text)
{
    m_textContent.setText(text);
    if (m_textContent.length() > static_cast<int>(m_maxLength))
    {
        m_textContent.truncate(static_cast<int>(m_maxLength));
    }
}

const CustomString& InputBox::getText() const
{
    return m_textContent;
}

bool InputBox::isFocused() const
{
    return m_isFocused;
}

sf::FloatRect InputBox::getBounds() const
{
    return m_shape.getGlobalBounds();
}

void InputBox::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        m_isFocused = m_shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    }

    if (!m_isFocused)
    {
        return;
    }

    if (event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode == 8)
        {
            const char* current = m_textContent.cStr();
            int len = m_textContent.length();

            if (len > 0)
            {
                char* newBuffer = new char[len];
                for (int i = 0; i < len - 1; i++)
                {
                    newBuffer[i] = current[i];
                }
                newBuffer[len - 1] = '\0';
                m_textContent.setText(newBuffer);
                delete[] newBuffer;
            }
        }
        else if (event.text.unicode >= 32 && event.text.unicode < 128)
        {
            if (m_textContent.length() < static_cast<int>(m_maxLength))
            {
                char c = static_cast<char>(event.text.unicode);
                char temp[2];
                temp[0] = c;
                temp[1] = '\0';
                m_textContent.append(temp);
            }
        }
    }
}

void InputBox::update()
{
    if (m_isPassword)
    {
        char* masked = new char[m_textContent.length() + 1];
        for (int i = 0; i < m_textContent.length(); i++)
        {
            masked[i] = '*';
        }
        masked[m_textContent.length()] = '\0';
        m_text.setString(masked);
        delete[] masked;
    }
    else
    {
        m_text.setString(m_textContent.cStr());
    }

    if (m_isFocused)
    {
        m_shape.setOutlineColor(sf::Color::Cyan);
    }
    else
    {
        m_shape.setOutlineColor(sf::Color::White);
    }
}

void InputBox::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);

    if (m_textContent.isEmpty() && !m_isFocused)
    {
        window.draw(m_placeholderText);
    }
    else
    {
        window.draw(m_text);
    }
}