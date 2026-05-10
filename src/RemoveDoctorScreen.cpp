#include "RemoveDoctorScreen.h"
#include <iostream>

RemoveDoctorScreen::RemoveDoctorScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_doctorIdInput()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::RemoveDoctor;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Remove Doctor", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Cannot remove a doctor with pending appointments.", 20, sf::Vector2f(70.f, 80.f));
    m_idLabel = Label(m_font, "Doctor ID", 22, sf::Vector2f(70.f, 250.f));
    m_listLabel = Label(m_font, "Existing Doctors", 24, sf::Vector2f(540.f, 30.f));
    m_headerLabel = Label(m_font, "ID | Name | Specialization | Fee", 18, sf::Vector2f(540.f, 80.f));

    m_doctorIdInput = InputBox(
        m_font,
        "Enter doctor ID",
        sf::Vector2f(70.f, 285.f),
        sf::Vector2f(380.f, 45.f),
        20,
        false,
        20
    );

    m_submitButton = Button(m_font, "Remove", sf::Vector2f(70.f, 365.f), sf::Vector2f(180.f, 50.f));
    m_backButton = Button(m_font, "Back", sf::Vector2f(270.f, 365.f), sf::Vector2f(180.f, 50.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 455.f), sf::Vector2f(430.f, 120.f), 18);
}

void RemoveDoctorScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void RemoveDoctorScreen::submitRemoval()
{
    const CustomString& doctorIdText = m_doctorIdInput.getText();

    if (doctorIdText.isEmpty())
    {
        m_messagePanel.setMessage("Please enter a doctor ID.");
        return;
    }

    if (!Validator::isValidId(doctorIdText.cStr()))
    {
        m_messagePanel.setMessage("Invalid doctor ID.");
        return;
    }

    int doctorId = 0;
    const char* idChars = doctorIdText.cStr();
    int i = 0;
    while (idChars[i] != '\0')
    {
        doctorId = doctorId * 10 + (idChars[i] - '0');
        i++;
    }

    try
    {
        bool removed = m_backend.removeDoctor(doctorId);

        if (removed)
        {
            m_messagePanel.setMessage("Doctor removed.");
        }
        else
        {
            m_messagePanel.setMessage("Doctor could not be removed.");
        }
    }
    catch (const HospitalException& ex)
    {
        m_messagePanel.setMessage(ex.what());
    }
}

void RemoveDoctorScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_doctorIdInput.handleEvent(event, window);
    m_submitButton.handleEvent(event, window);
    m_backButton.handleEvent(event, window);

    if (m_submitButton.isClicked(event, window))
    {
        submitRemoval();
    }
    else if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::AdminDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        submitRemoval();
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::AdminDashboard);
    }
}

void RemoveDoctorScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
    m_doctorIdInput.update();
}

void RemoveDoctorScreen::renderDoctorList(sf::RenderWindow& window) const
{
    const Doctor* doctorData = m_backend.getDoctors().getAll();

    if (m_backend.getDoctors().size() == 0)
    {
        sf::Text none;
        none.setFont(m_font);
        none.setCharacterSize(18);
        none.setFillColor(sf::Color::White);
        none.setPosition(540.f, 120.f);
        none.setString("No doctors found.");
        window.draw(none);
        return;
    }

    float y = 120.f;
    int visibleCount = 0;

    for (int i = 0; i < m_backend.getDoctors().size(); i++)
    {
        const Doctor& doctor = doctorData[i];

        sf::Text row;
        row.setFont(m_font);
        row.setCharacterSize(16);
        row.setFillColor(sf::Color::White);
        row.setPosition(540.f, y);

        CustomString line;
        line.setText("");

        char idText[32];
        int idValue = doctor.getId();
        int idx = 0;

        if (idValue == 0)
        {
            idText[idx++] = '0';
        }
        else
        {
            char reversed[32];
            int r = 0;
            while (idValue > 0 && r < 31)
            {
                reversed[r++] = static_cast<char>('0' + (idValue % 10));
                idValue /= 10;
            }

            for (int j = r - 1; j >= 0; j--)
            {
                idText[idx++] = reversed[j];
            }
        }
        idText[idx] = '\0';

        line += idText;
        line += " | ";
        line += doctor.getName().cStr();
        line += " | ";
        line += doctor.getSpecialization().cStr();
        line += " | ";
        line += doctor.getFee();

        row.setString(line.cStr());
        window.draw(row);

        y += 26.f;
        visibleCount++;

        if (visibleCount >= 12)
        {
            break;
        }
    }
}

void RemoveDoctorScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);
    m_idLabel.draw(window);
    m_listLabel.draw(window);
    m_headerLabel.draw(window);

    m_doctorIdInput.draw(window);
    m_submitButton.draw(window);
    m_backButton.draw(window);

    m_messagePanel.draw(window);

    renderDoctorList(window);
}

bool RemoveDoctorScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState RemoveDoctorScreen::getRequestedState() const
{
    return m_requestedState;
}

void RemoveDoctorScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}