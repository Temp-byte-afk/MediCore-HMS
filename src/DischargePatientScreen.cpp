#include "DischargePatientScreen.h"
#include <iostream>

DischargePatientScreen::DischargePatientScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_patientIdInput()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::DischargePatient;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Discharge Patient", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Patient must have no unpaid bills and no pending appointments.", 20, sf::Vector2f(70.f, 80.f));
    m_idLabel = Label(m_font, "Patient ID", 22, sf::Vector2f(70.f, 250.f));
    m_listLabel = Label(m_font, "Existing Patients", 24, sf::Vector2f(540.f, 30.f));
    m_headerLabel = Label(m_font, "ID | Name | Unpaid Bills | Pending Appointments", 18, sf::Vector2f(540.f, 80.f));

    m_patientIdInput = InputBox(
        m_font,
        "Enter patient ID",
        sf::Vector2f(70.f, 285.f),
        sf::Vector2f(380.f, 45.f),
        20,
        false,
        20
    );

    m_submitButton = Button(m_font, "Discharge", sf::Vector2f(70.f, 365.f), sf::Vector2f(180.f, 50.f));
    m_backButton = Button(m_font, "Back", sf::Vector2f(270.f, 365.f), sf::Vector2f(180.f, 50.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 455.f), sf::Vector2f(430.f, 120.f), 18);
}

void DischargePatientScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void DischargePatientScreen::appendIntToBuffer(int value, char* buffer, int maxLength)
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

void DischargePatientScreen::appendDoubleToBuffer(double value, char* buffer, int maxLength)
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

void DischargePatientScreen::submitDischarge()
{
    const CustomString& patientIdText = m_patientIdInput.getText();

    if (patientIdText.isEmpty())
    {
        m_messagePanel.setMessage("Please enter a patient ID.");
        return;
    }

    if (!Validator::isValidId(patientIdText.cStr()))
    {
        m_messagePanel.setMessage("Invalid patient ID.");
        return;
    }

    int patientId = 0;
    const char* idChars = patientIdText.cStr();
    int i = 0;
    while (idChars[i] != '\0')
    {
        patientId = patientId * 10 + (idChars[i] - '0');
        i++;
    }

    try
    {
        bool discharged = m_backend.dischargePatient(patientId);

        if (discharged)
        {
            m_messagePanel.setMessage("Patient discharged and archived successfully.");
        }
        else
        {
            m_messagePanel.setMessage("Patient could not be discharged.");
        }
    }
    catch (const HospitalException& ex)
    {
        m_messagePanel.setMessage(ex.what());
    }
}

void DischargePatientScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_patientIdInput.handleEvent(event, window);
    m_submitButton.handleEvent(event, window);
    m_backButton.handleEvent(event, window);

    if (m_submitButton.isClicked(event, window))
    {
        submitDischarge();
    }
    else if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::AdminDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        submitDischarge();
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::AdminDashboard);
    }
}

void DischargePatientScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
    m_patientIdInput.update();
}

void DischargePatientScreen::renderPatientList(sf::RenderWindow& window) const
{
    const Patient* patientData = m_backend.getPatients().getAll();

    if (m_backend.getPatients().size() == 0)
    {
        sf::Text none;
        none.setFont(m_font);
        none.setCharacterSize(18);
        none.setFillColor(sf::Color::White);
        none.setPosition(540.f, 120.f);
        none.setString("No patients found.");
        window.draw(none);
        return;
    }

    float y = 120.f;
    int visibleCount = 0;

    for (int i = 0; i < m_backend.getPatients().size(); i++)
    {
        const Patient& patient = patientData[i];

        sf::Text row;
        row.setFont(m_font);
        row.setCharacterSize(16);
        row.setFillColor(sf::Color::White);
        row.setPosition(540.f, y);

        CustomString line;
        line.setText("");

        char idText[32];
        char unpaidText[32];
        char pendingText[32];

        appendIntToBuffer(patient.getId(), idText, 32);
        appendIntToBuffer(m_backend.getUnpaidBillCountForPatient(patient.getId()), unpaidText, 32);

        int pendingCount = 0;
        const Appointment* appointmentData = m_backend.getAppointments().getAll();
        for (int a = 0; a < m_backend.getAppointments().size(); a++)
        {
            if (appointmentData[a].getPatientId() == patient.getId() && appointmentData[a].isPending())
            {
                pendingCount++;
            }
        }
        appendIntToBuffer(pendingCount, pendingText, 32);

        line += idText;
        line += " | ";
        line += patient.getName().cStr();
        line += " | ";
        line += unpaidText;
        line += " | ";
        line += pendingText;

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

void DischargePatientScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);
    m_idLabel.draw(window);
    m_listLabel.draw(window);
    m_headerLabel.draw(window);

    m_patientIdInput.draw(window);
    m_submitButton.draw(window);
    m_backButton.draw(window);

    m_messagePanel.draw(window);

    renderPatientList(window);
}

bool DischargePatientScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState DischargePatientScreen::getRequestedState() const
{
    return m_requestedState;
}

void DischargePatientScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}