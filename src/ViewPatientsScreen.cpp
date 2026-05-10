#include "ViewPatientsScreen.h"
#include <iostream>

ViewPatientsScreen::ViewPatientsScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_backButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::ViewAllPatients;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "View All Patients", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Includes balance and unpaid bill count", 20, sf::Vector2f(70.f, 80.f));
    m_headerLabel = Label(m_font, "ID | Name | Age | Gender | Contact | Balance | Unpaid Bills", 18, sf::Vector2f(70.f, 140.f));

    m_backButton = Button(m_font, "Back", sf::Vector2f(70.f, 650.f), sf::Vector2f(180.f, 45.f));
    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 590.f), sf::Vector2f(750.f, 45.f), 18);
}

void ViewPatientsScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void ViewPatientsScreen::appendIntToBuffer(int value, char* buffer, int maxLength)
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

void ViewPatientsScreen::appendDoubleToBuffer(double value, char* buffer, int maxLength)
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

    appendIntToBuffer(wholePart, buffer + index, maxLength - index);
    while (buffer[index] != '\0' && index < maxLength - 1)
    {
        index++;
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

void ViewPatientsScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
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

void ViewPatientsScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
}

void ViewPatientsScreen::renderPatientList(sf::RenderWindow& window) const
{
    const Patient* patientData = m_backend.getPatients().getAll();

    if (m_backend.getPatients().size() == 0)
    {
        sf::Text none;
        none.setFont(m_font);
        none.setCharacterSize(18);
        none.setFillColor(sf::Color::White);
        none.setPosition(70.f, 190.f);
        none.setString("No patients found.");
        window.draw(none);
        return;
    }

    float y = 190.f;
    int visibleCount = 0;

    for (int i = 0; i < m_backend.getPatients().size(); i++)
    {
        const Patient& patient = patientData[i];

        sf::Text row;
        row.setFont(m_font);
        row.setCharacterSize(16);
        row.setFillColor(sf::Color::White);
        row.setPosition(70.f, y);

        CustomString line;
        line.setText("");

        char idText[32];
        char ageText[32];
        char balanceText[64];
        char unpaidText[32];

        appendIntToBuffer(patient.getId(), idText, 32);
        appendIntToBuffer(patient.getAge(), ageText, 32);
        appendDoubleToBuffer(patient.getBalance(), balanceText, 64);
        appendIntToBuffer(m_backend.getUnpaidBillCountForPatient(patient.getId()), unpaidText, 32);

        line += idText;
        line += " | ";
        line += patient.getName().cStr();
        line += " | ";
        line += ageText;
        line += " | ";
        line += patient.getGender().cStr();
        line += " | ";
        line += patient.getContact().cStr();
        line += " | ";
        line += balanceText;
        line += " | ";
        line += unpaidText;

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

void ViewPatientsScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);
    m_headerLabel.draw(window);

    renderPatientList(window);

    m_backButton.draw(window);
    m_messagePanel.draw(window);
}

bool ViewPatientsScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState ViewPatientsScreen::getRequestedState() const
{
    return m_requestedState;
}

void ViewPatientsScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}