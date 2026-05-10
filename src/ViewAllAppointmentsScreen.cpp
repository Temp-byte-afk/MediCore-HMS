#include "ViewAllAppointmentsScreen.h"
#include <iostream>
#include "DateUtils.h"

ViewAllAppointmentsScreen::ViewAllAppointmentsScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_backButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::ViewAllAppointments;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "View All Appointments", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Sorted by date descending", 20, sf::Vector2f(70.f, 80.f));
    m_headerLabel = Label(m_font, "ID | Patient Name | Doctor Name | Date | Time Slot | Status", 18, sf::Vector2f(70.f, 140.f));

    m_backButton = Button(m_font, "Back", sf::Vector2f(70.f, 650.f), sf::Vector2f(180.f, 45.f));
    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 590.f), sf::Vector2f(820.f, 45.f), 18);
}

void ViewAllAppointmentsScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void ViewAllAppointmentsScreen::appendIntToBuffer(int value, char* buffer, int maxLength)
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

int ViewAllAppointmentsScreen::compareTimeSlots(const CustomString& first, const CustomString& second) const
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

int ViewAllAppointmentsScreen::compareAppointmentsForDisplay(const Appointment& first, const Appointment& second) const
{
    int dateCompare = DateUtils::compareDates(first.getDate().cStr(), second.getDate().cStr());

    if (dateCompare != 0)
    {
        return -dateCompare; // descending
    }

    return compareTimeSlots(first.getTimeSlot(), second.getTimeSlot());
}

void ViewAllAppointmentsScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
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

void ViewAllAppointmentsScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
}

void ViewAllAppointmentsScreen::renderAppointmentList(sf::RenderWindow& window) const
{
    const Appointment* appointmentData = m_backend.getAppointments().getAll();
    const Patient* patientData = m_backend.getPatients().getAll();
    const Doctor* doctorData = m_backend.getDoctors().getAll();

    int count = m_backend.getAppointments().size();

    if (count == 0)
    {
        sf::Text none;
        none.setFont(m_font);
        none.setCharacterSize(18);
        none.setFillColor(sf::Color::White);
        none.setPosition(70.f, 190.f);
        none.setString("No appointments found.");
        window.draw(none);
        return;
    }

    const Appointment** sortedAppointments = new const Appointment * [count];
    for (int i = 0; i < count; i++)
    {
        sortedAppointments[i] = &appointmentData[i];
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (compareAppointmentsForDisplay(*sortedAppointments[j], *sortedAppointments[j + 1]) > 0)
            {
                const Appointment* temp = sortedAppointments[j];
                sortedAppointments[j] = sortedAppointments[j + 1];
                sortedAppointments[j + 1] = temp;
            }
        }
    }

    float y = 190.f;
    int visibleCount = 0;

    for (int i = 0; i < count; i++)
    {
        const Appointment& appointment = *sortedAppointments[i];
        const Patient* patient = nullptr;
        const Doctor* doctor = nullptr;

        for (int p = 0; p < m_backend.getPatients().size(); p++)
        {
            if (patientData[p].getId() == appointment.getPatientId())
            {
                patient = &patientData[p];
                break;
            }
        }

        for (int d = 0; d < m_backend.getDoctors().size(); d++)
        {
            if (doctorData[d].getId() == appointment.getDoctorId())
            {
                doctor = &doctorData[d];
                break;
            }
        }

        sf::Text row;
        row.setFont(m_font);
        row.setCharacterSize(16);
        row.setFillColor(sf::Color::White);
        row.setPosition(70.f, y);

        CustomString line;
        line.setText("");

        char idText[32];
        appendIntToBuffer(appointment.getAppointmentId(), idText, 32);

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
        line += " | ";
        line += appointment.getStatus().cStr();

        row.setString(line.cStr());
        window.draw(row);

        y += 28.f;
        visibleCount++;

        if (visibleCount >= 14)
        {
            break;
        }
    }

    delete[] sortedAppointments;
}

void ViewAllAppointmentsScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);
    m_headerLabel.draw(window);

    renderAppointmentList(window);

    m_backButton.draw(window);
    m_messagePanel.draw(window);
}

bool ViewAllAppointmentsScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState ViewAllAppointmentsScreen::getRequestedState() const
{
    return m_requestedState;
}

void ViewAllAppointmentsScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}