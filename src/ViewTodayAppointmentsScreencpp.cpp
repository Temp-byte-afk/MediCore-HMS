#include "ViewTodayAppointmentsScreen.h"
#include "DateUtils.h"
#include <iostream>

ViewTodayAppointmentsScreen::ViewTodayAppointmentsScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_backButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::ViewTodayAppointments;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Today's Appointments", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Sorted by time slot ascending", 20, sf::Vector2f(70.f, 80.f));
    m_doctorLabel = Label(m_font, "Doctor: ", 22, sf::Vector2f(70.f, 115.f));
    m_headerLabel = Label(m_font, "Appointment ID | Patient Name | Time Slot | Status", 18, sf::Vector2f(70.f, 180.f));

    m_backButton = Button(m_font, "Back", sf::Vector2f(70.f, 640.f), sf::Vector2f(180.f, 45.f));
    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 565.f), sf::Vector2f(650.f, 55.f), 18);
}

void ViewTodayAppointmentsScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

int ViewTodayAppointmentsScreen::compareTimeSlots(const CustomString& first, const CustomString& second) const
{
    const char* a = first.cStr();
    const char* b = second.cStr();

    int i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] < b[i])
        {
            return -1;
        }
        if (a[i] > b[i])
        {
            return 1;
        }
        i++;
    }

    if (a[i] == '\0' && b[i] == '\0')
    {
        return 0;
    }

    return (a[i] == '\0') ? -1 : 1;
}

void ViewTodayAppointmentsScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_backButton.handleEvent(event, window);

    if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::DoctorDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::DoctorDashboard);
    }
}

void ViewTodayAppointmentsScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
}

void ViewTodayAppointmentsScreen::renderAppointmentList(sf::RenderWindow& window) const
{
    int doctorId = m_backend.getCurrentUserId();
    char today[11];
    DateUtils::getCurrentDate(today, 11);

    const Appointment* appointmentData = m_backend.getAppointments().getAll();
    const Patient* patientData = m_backend.getPatients().getAll();

    int count = 0;
    for (int i = 0; i < m_backend.getAppointments().size(); i++)
    {
        if (appointmentData[i].getDoctorId() == doctorId &&
            appointmentData[i].getDate().equals(today))
        {
            count++;
        }
    }

    if (count == 0)
    {
        sf::Text none;
        none.setFont(m_font);
        none.setCharacterSize(18);
        none.setFillColor(sf::Color::White);
        none.setPosition(70.f, 220.f);
        none.setString("No appointments scheduled for today.");
        window.draw(none);
        return;
    }

    const Appointment** sortedAppointments = new const Appointment * [count];
    int index = 0;

    for (int i = 0; i < m_backend.getAppointments().size(); i++)
    {
        if (appointmentData[i].getDoctorId() == doctorId &&
            appointmentData[i].getDate().equals(today))
        {
            sortedAppointments[index] = &appointmentData[i];
            index++;
        }
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (compareTimeSlots(sortedAppointments[j]->getTimeSlot(), sortedAppointments[j + 1]->getTimeSlot()) > 0)
            {
                const Appointment* temp = sortedAppointments[j];
                sortedAppointments[j] = sortedAppointments[j + 1];
                sortedAppointments[j + 1] = temp;
            }
        }
    }

    float y = 220.f;
    int visibleCount = 0;

    for (int i = 0; i < count; i++)
    {
        const Appointment& appointment = *sortedAppointments[i];
        const Patient* patient = nullptr;

        for (int j = 0; j < m_backend.getPatients().size(); j++)
        {
            if (patientData[j].getId() == appointment.getPatientId())
            {
                patient = &patientData[j];
                break;
            }
        }

        sf::Text row;
        row.setFont(m_font);
        row.setCharacterSize(17);
        row.setFillColor(sf::Color::White);
        row.setPosition(70.f, y);

        CustomString line;
        line.setText("");

        char idText[32];
        int idValue = appointment.getAppointmentId();
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
            for (int k = r - 1; k >= 0; k--)
            {
                idText[idx++] = reversed[k];
            }
        }
        idText[idx] = '\0';

        line += idText;
        line += " | ";

        if (patient != nullptr)
        {
            line += patient->getName().cStr();
        }
        else
        {
            line += "Unknown Patient";
        }

        line += " | ";
        line += appointment.getTimeSlot().cStr();
        line += " | ";
        line += appointment.getStatus().cStr();

        row.setString(line.cStr());
        window.draw(row);

        y += 28.f;
        visibleCount++;

        if (visibleCount >= 12)
        {
            break;
        }
    }

    delete[] sortedAppointments;
}

void ViewTodayAppointmentsScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);

    CustomString doctorText("Doctor: ");
    doctorText += m_backend.getCurrentUserName();
    m_doctorLabel.setText(doctorText.cStr());
    m_doctorLabel.draw(window);

    m_headerLabel.draw(window);

    renderAppointmentList(window);

    m_backButton.draw(window);
    m_messagePanel.draw(window);
}

bool ViewTodayAppointmentsScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState ViewTodayAppointmentsScreen::getRequestedState() const
{
    return m_requestedState;
}

void ViewTodayAppointmentsScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}