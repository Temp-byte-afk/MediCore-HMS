#include "ViewDoctorsScreen.h"
#include <iostream>

ViewDoctorsScreen::ViewDoctorsScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_backButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::ViewAllDoctors;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "View All Doctors", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "All registered doctors in MediCore", 20, sf::Vector2f(70.f, 80.f));
    m_headerLabel = Label(m_font, "ID | Name | Specialization | Contact | Fee", 18, sf::Vector2f(70.f, 140.f));

    m_backButton = Button(m_font, "Back", sf::Vector2f(70.f, 650.f), sf::Vector2f(180.f, 45.f));
    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 590.f), sf::Vector2f(750.f, 45.f), 18);
}

void ViewDoctorsScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void ViewDoctorsScreen::appendIntToBuffer(int value, char* buffer, int maxLength)
{
    if (buffer == nullptr || maxLength <= 0)
    {
        return;
    }

    int index = 0;

    if (value == 0)
    {
        if (index < maxLength - 1)
        {
            buffer[index++] = '0';
        }
        buffer[index] = '\0';
        return;
    }

    if (value < 0)
    {
        if (index < maxLength - 1)
        {
            buffer[index++] = '-';
        }
        value = -value;
    }

    char reversed[32];
    int r = 0;
    while (value > 0 && r < 31)
    {
        reversed[r++] = static_cast<char>('0' + (value % 10));
        value /= 10;
    }

    for (int i = r - 1; i >= 0 && index < maxLength - 1; i--)
    {
        buffer[index++] = reversed[i];
    }

    buffer[index] = '\0';
}

void ViewDoctorsScreen::appendDoubleToBuffer(double value, char* buffer, int maxLength)
{
    if (buffer == nullptr || maxLength <= 0)
    {
        return;
    }

    int index = 0;

    if (value < 0.0)
    {
        if (index < maxLength - 1)
        {
            buffer[index++] = '-';
        }
        value = -value;
    }

    int wholePart = static_cast<int>(value);
    int fractionalPart = static_cast<int>((value - wholePart) * 100.0 + 0.5);

    if (fractionalPart >= 100)
    {
        wholePart += 1;
        fractionalPart -= 100;
    }

    if (wholePart == 0)
    {
        if (index < maxLength - 1)
        {
            buffer[index++] = '0';
        }
    }
    else
    {
        char reversed[32];
        int r = 0;
        while (wholePart > 0 && r < 31)
        {
            reversed[r++] = static_cast<char>('0' + (wholePart % 10));
            wholePart /= 10;
        }

        for (int i = r - 1; i >= 0 && index < maxLength - 1; i--)
        {
            buffer[index++] = reversed[i];
        }
    }

    if (index < maxLength - 1)
    {
        buffer[index++] = '.';
    }

    if (fractionalPart < 10)
    {
        if (index < maxLength - 1)
        {
            buffer[index++] = '0';
        }
        if (index < maxLength - 1)
        {
            buffer[index++] = static_cast<char>('0' + fractionalPart);
        }
    }
    else
    {
        if (index < maxLength - 1)
        {
            buffer[index++] = static_cast<char>('0' + (fractionalPart / 10));
        }
        if (index < maxLength - 1)
        {
            buffer[index++] = static_cast<char>('0' + (fractionalPart % 10));
        }
    }

    buffer[index] = '\0';
}

void ViewDoctorsScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_backButton.handleEvent(event, window);

    if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::AdminDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::AdminDashboard);
    }
}

void ViewDoctorsScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
}

void ViewDoctorsScreen::renderDoctorList(sf::RenderWindow& window) const
{
    const Doctor* doctorData = m_backend.getDoctors().getAll();

    if (m_backend.getDoctors().size() == 0)
    {
        sf::Text none;
        none.setFont(m_font);
        none.setCharacterSize(18);
        none.setFillColor(sf::Color::White);
        none.setPosition(70.f, 190.f);
        none.setString("No doctors found.");
        window.draw(none);
        return;
    }

    float y = 190.f;
    int visibleCount = 0;

    for (int i = 0; i < m_backend.getDoctors().size(); i++)
    {
        const Doctor& doctor = doctorData[i];

        sf::Text row;
        row.setFont(m_font);
        row.setCharacterSize(16);
        row.setFillColor(sf::Color::White);
        row.setPosition(70.f, y);

        CustomString line;
        line.setText("");

        char idText[32];
        appendIntToBuffer(doctor.getId(), idText, 32);

        char feeText[64];
        appendDoubleToBuffer(doctor.getFee(), feeText, 64);

        line += idText;
        line += " | ";
        line += doctor.getName().cStr();
        line += " | ";
        line += doctor.getSpecialization().cStr();
        line += " | ";
        line += doctor.getContact().cStr();
        line += " | ";
        line += feeText;

        row.setString(line.cStr());
        window.draw(row);

        y += 28.f;
        visibleCount++;

        if (visibleCount >= 14)
        {
            break;
        }
    }
}

void ViewDoctorsScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);
    m_headerLabel.draw(window);

    renderDoctorList(window);

    m_backButton.draw(window);
    m_messagePanel.draw(window);
}

bool ViewDoctorsScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState ViewDoctorsScreen::getRequestedState() const
{
    return m_requestedState;
}

void ViewDoctorsScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}