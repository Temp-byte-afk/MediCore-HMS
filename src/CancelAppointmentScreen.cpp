#include "CancelAppointmentScreen.h"
#include <iostream>

CancelAppointmentScreen::CancelAppointmentScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_appointmentIdInput(),
    m_submitButton(),
    m_backButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::CancelAppointment;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Cancel Appointment", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Only pending appointments can be cancelled.", 20, sf::Vector2f(70.f, 80.f));
    m_idLabel = Label(m_font, "Appointment ID", 22, sf::Vector2f(70.f, 250.f));
    m_listLabel = Label(m_font, "Your Pending Appointments", 24, sf::Vector2f(540.f, 30.f));

    m_appointmentIdInput = InputBox(
        m_font,
        "Enter appointment ID",
        sf::Vector2f(70.f, 285.f),
        sf::Vector2f(380.f, 45.f),
        20,
        false,
        20
    );

    m_submitButton = Button(m_font, "Cancel", sf::Vector2f(70.f, 365.f), sf::Vector2f(180.f, 50.f));
    m_backButton = Button(m_font, "Back", sf::Vector2f(270.f, 365.f), sf::Vector2f(180.f, 50.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 455.f), sf::Vector2f(430.f, 120.f), 18);
}

void CancelAppointmentScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void CancelAppointmentScreen::submitCancellation()
{
    const CustomString& appointmentIdText = m_appointmentIdInput.getText();

    if (appointmentIdText.isEmpty())
    {
        m_messagePanel.setMessage("Please enter an appointment ID.");
        return;
    }

    if (!Validator::isValidId(appointmentIdText.cStr()))
    {
        m_messagePanel.setMessage("Invalid appointment ID.");
        return;
    }

    int appointmentId = 0;
    const char* idChars = appointmentIdText.cStr();
    int i = 0;
    while (idChars[i] != '\0')
    {
        appointmentId = appointmentId * 10 + (idChars[i] - '0');
        i++;
    }

    int patientId = m_backend.getCurrentUserId();

    try
    {
        double refundedAmount = m_backend.cancelAppointment(patientId, appointmentId);

        char message[160];
        char amountText[32];

        // manual double-to-text for simple display
        int whole = static_cast<int>(refundedAmount);
        int idx = 0;

        if (whole == 0)
        {
            amountText[idx++] = '0';
        }
        else
        {
            char reversed[32];
            int r = 0;
            while (whole > 0 && r < 31)
            {
                reversed[r++] = static_cast<char>('0' + (whole % 10));
                whole /= 10;
            }

            for (int j = r - 1; j >= 0; j--)
            {
                amountText[idx++] = reversed[j];
            }
        }
        amountText[idx] = '\0';

        const char* prefix = "Appointment cancelled. PKR ";
        int k = 0;
        while (prefix[k] != '\0')
        {
            message[k] = prefix[k];
            k++;
        }

        int a = 0;
        while (amountText[a] != '\0')
        {
            message[k++] = amountText[a++];
        }

        const char* suffix = " refunded to your balance.";
        int s = 0;
        while (suffix[s] != '\0')
        {
            message[k++] = suffix[s++];
        }

        message[k] = '\0';
        m_messagePanel.setMessage(message);
    }
    catch (const HospitalException& ex)
    {
        m_messagePanel.setMessage(ex.what());
    }
}

void CancelAppointmentScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_appointmentIdInput.handleEvent(event, window);
    m_submitButton.handleEvent(event, window);
    m_backButton.handleEvent(event, window);

    if (m_submitButton.isClicked(event, window))
    {
        submitCancellation();
    }
    else if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::PatientDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        submitCancellation();
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::PatientDashboard);
    }
}

void CancelAppointmentScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
    m_appointmentIdInput.update();
}

void CancelAppointmentScreen::renderPendingAppointments(sf::RenderWindow& window) const
{
    const int patientId = m_backend.getCurrentUserId();
    const Appointment* appointmentData = m_backend.getAppointments().getAll();
    const Doctor* doctorData = m_backend.getDoctors().getAll();

    bool found = false;
    float y = 110.f;

    Label header(m_font, "Appointment ID | Doctor Name | Date | Time Slot", 18, sf::Vector2f(540.f, y));
    header.draw(window);
    y += 30.f;

    for (int i = 0; i < m_backend.getAppointments().size(); i++)
    {
        const Appointment& appointment = appointmentData[i];

        if (appointment.getPatientId() == patientId && appointment.isPending())
        {
            const Doctor* doctor = nullptr;
            for (int j = 0; j < m_backend.getDoctors().size(); j++)
            {
                if (doctorData[j].getId() == appointment.getDoctorId())
                {
                    doctor = &doctorData[j];
                    break;
                }
            }

            sf::Text text;
            text.setFont(m_font);
            text.setCharacterSize(18);
            text.setFillColor(sf::Color::White);
            text.setPosition(540.f, y);

            CustomString line;
            line.setText("");

            char idBuf[32];
            int idValue = appointment.getAppointmentId();
            int idx = 0;
            if (idValue == 0)
            {
                idBuf[idx++] = '0';
            }
            else
            {
                char rev[32];
                int r = 0;
                while (idValue > 0 && r < 31)
                {
                    rev[r++] = static_cast<char>('0' + (idValue % 10));
                    idValue /= 10;
                }
                for (int t = r - 1; t >= 0; t--)
                {
                    idBuf[idx++] = rev[t];
                }
            }
            idBuf[idx] = '\0';

            line += idBuf;
            line += " | ";

            if (doctor != nullptr)
            {
                line += doctor->getName().cStr();
            }
            else
            {
                line += "Unknown Doctor";
            }

            line += " | ";
            line += appointment.getDate().cStr();
            line += " | ";
            line += appointment.getTimeSlot().cStr();

            text.setString(line.cStr());
            window.draw(text);

            y += 28.f;
            found = true;
        }
    }

    if (!found)
    {
        sf::Text none;
        none.setFont(m_font);
        none.setCharacterSize(18);
        none.setFillColor(sf::Color::White);
        none.setPosition(540.f, y);
        none.setString("You have no pending appointments.");
        window.draw(none);
    }
}

void CancelAppointmentScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);
    m_idLabel.draw(window);
    m_listLabel.draw(window);

    m_appointmentIdInput.draw(window);
    m_submitButton.draw(window);
    m_backButton.draw(window);
    m_messagePanel.draw(window);

    renderPendingAppointments(window);
}

bool CancelAppointmentScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState CancelAppointmentScreen::getRequestedState() const
{
    return m_requestedState;
}

void CancelAppointmentScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}