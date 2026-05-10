#include "BookAppointmentScreen.h"
#include <iostream>

BookAppointmentScreen::BookAppointmentScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_specializationInput(),
    m_doctorIdInput(),
    m_dateInput(),
    m_timeSlotInput(),
    m_submitButton(),
    m_backButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::BookAppointment;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Book Appointment", 34, sf::Vector2f(70.f, 30.f));
    m_specializationLabel = Label(m_font, "Specialization", 22, sf::Vector2f(70.f, 120.f));
    m_doctorIdLabel = Label(m_font, "Doctor ID", 22, sf::Vector2f(70.f, 210.f));
    m_dateLabel = Label(m_font, "Date (DD-MM-YYYY)", 22, sf::Vector2f(70.f, 300.f));
    m_timeSlotLabel = Label(m_font, "Time Slot (e.g. 09:00)", 22, sf::Vector2f(70.f, 390.f));

    m_specializationInput = InputBox(m_font, "Enter specialization", sf::Vector2f(70.f, 155.f), sf::Vector2f(420.f, 45.f), 20, false, 50);
    m_doctorIdInput = InputBox(m_font, "Enter doctor ID", sf::Vector2f(70.f, 245.f), sf::Vector2f(420.f, 45.f), 20, false, 20);
    m_dateInput = InputBox(m_font, "Enter date", sf::Vector2f(70.f, 335.f), sf::Vector2f(420.f, 45.f), 20, false, 20);
    m_timeSlotInput = InputBox(m_font, "Enter time slot", sf::Vector2f(70.f, 425.f), sf::Vector2f(420.f, 45.f), 20, false, 10);

    m_submitButton = Button(m_font, "Book", sf::Vector2f(70.f, 510.f), sf::Vector2f(180.f, 50.f));
    m_backButton = Button(m_font, "Back", sf::Vector2f(270.f, 510.f), sf::Vector2f(180.f, 50.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(540.f, 150.f), sf::Vector2f(650.f, 140.f), 20);
}

void BookAppointmentScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

//void BookAppointmentScreen::submitBooking()
//{
//    const CustomString& specialization = m_specializationInput.getText();
//    const CustomString& doctorIdText = m_doctorIdInput.getText();
//    const CustomString& dateText = m_dateInput.getText();
//    const CustomString& timeSlotText = m_timeSlotInput.getText();
//
//    if (specialization.isEmpty() || doctorIdText.isEmpty() || dateText.isEmpty() || timeSlotText.isEmpty())
//    {
//        m_messagePanel.setMessage("Please fill all fields.");
//        return;
//    }
//
//    if (!Validator::isValidId(doctorIdText.cStr()))
//    {
//        m_messagePanel.setMessage("Invalid Doctor ID.");
//        return;
//    }
//
//    int doctorId = 0;
//    const char* idChars = doctorIdText.cStr();
//    int i = 0;
//    while (idChars[i] != '\0')
//    {
//        doctorId = doctorId * 10 + (idChars[i] - '0');
//        i++;
//    }
//
//    int patientId = m_backend.getCurrentUserId();
//
//    try
//    {
//        int appointmentId = m_backend.bookAppointment(
//            patientId,
//            doctorId,
//            dateText.cStr(),
//            timeSlotText.cStr()
//        );
//
//        m_messagePanel.setMessage("Appointment booked successfully.");
//
//        char resultText[128];
//        char idBuffer[32];
//
//        int index = 0;
//        int tempId = appointmentId;
//        char reversed[32];
//
//        if (tempId == 0)
//        {
//            reversed[index++] = '0';
//        }
//        else
//        {
//            while (tempId > 0 && index < 31)
//            {
//                reversed[index++] = static_cast<char>('0' + (tempId % 10));
//                tempId /= 10;
//            }
//        }
//
//        int outIndex = 0;
//        for (int j = index - 1; j >= 0; j--)
//        {
//            idBuffer[outIndex++] = reversed[j];
//        }
//        idBuffer[outIndex] = '\0';
//
//        int k = 0;
//        while ("Appointment ID: "[k] != '\0')
//        {
//            resultText[k] = "Appointment ID: "[k];
//            k++;
//        }
//
//        int r = 0;
//        while (idBuffer[r] != '\0' && k < 127)
//        {
//            resultText[k++] = idBuffer[r++];
//        }
//        resultText[k] = '\0';
//
//        m_messagePanel.setMessage(resultText);
//    }
//    catch (const HospitalException& ex)
//    {
//        m_messagePanel.setMessage(ex.what());
//    }
//}

void BookAppointmentScreen::submitBooking()
{
    const CustomString& specialization = m_specializationInput.getText();
    const CustomString& doctorIdText = m_doctorIdInput.getText();
    const CustomString& dateText = m_dateInput.getText();
    const CustomString& timeSlotText = m_timeSlotInput.getText();

    if (specialization.isEmpty() || doctorIdText.isEmpty() || dateText.isEmpty() || timeSlotText.isEmpty())
    {
        m_messagePanel.setMessage("Please fill all fields.");
        return;
    }

    if (!hasDoctorsForSpecialization(specialization.cStr()))
    {
        m_messagePanel.setMessage("No doctors available for that specialization.");
        return;
    }

    if (!Validator::isValidId(doctorIdText.cStr()))
    {
        m_messagePanel.setMessage("Invalid Doctor ID.");
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

    const Doctor* doctor = m_backend.getDoctors().findById(doctorId);
    if (doctor == nullptr)
    {
        m_messagePanel.setMessage("Doctor not found.");
        return;
    }

    if (!doctor->getSpecialization().equalsIgnoreCase(specialization.cStr()))
    {
        m_messagePanel.setMessage("Doctor does not match selected specialization.");
        return;
    }

    if (!Validator::isValidDate(dateText.cStr()))
    {
        m_messagePanel.setMessage("Invalid date.");
        return;
    }

    if (!Validator::isValidTimeSlot(timeSlotText.cStr()))
    {
        m_messagePanel.setMessage("Invalid time slot.");
        return;
    }

    int patientId = m_backend.getCurrentUserId();

    try
    {
        int appointmentId = m_backend.bookAppointment(
            patientId,
            doctorId,
            dateText.cStr(),
            timeSlotText.cStr()
        );

        char idBuffer[32];
        int tempId = appointmentId;
        int index = 0;

        if (tempId == 0)
        {
            idBuffer[index++] = '0';
        }
        else
        {
            char reversed[32];
            int reverseIndex = 0;

            while (tempId > 0 && reverseIndex < 31)
            {
                reversed[reverseIndex++] = static_cast<char>('0' + (tempId % 10));
                tempId /= 10;
            }

            for (int j = reverseIndex - 1; j >= 0; j--)
            {
                idBuffer[index++] = reversed[j];
            }
        }

        idBuffer[index] = '\0';

        CustomString resultText("Appointment booked successfully. Appointment ID: ");
        resultText += idBuffer;
        m_messagePanel.setMessage(resultText.cStr());
    }
    catch (const HospitalException& ex)
    {
        m_messagePanel.setMessage(ex.what());
    }
}

void BookAppointmentScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_specializationInput.handleEvent(event, window);
    m_doctorIdInput.handleEvent(event, window);
    m_dateInput.handleEvent(event, window);
    m_timeSlotInput.handleEvent(event, window);

    m_submitButton.handleEvent(event, window);
    m_backButton.handleEvent(event, window);

    if (m_submitButton.isClicked(event, window))
    {
        submitBooking();
    }
    else if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::PatientDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        submitBooking();
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::PatientDashboard);
    }
}

void BookAppointmentScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;

    m_specializationInput.update();
    m_doctorIdInput.update();
    m_dateInput.update();
    m_timeSlotInput.update();
}

void BookAppointmentScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_specializationLabel.draw(window);
    m_doctorIdLabel.draw(window);
    m_dateLabel.draw(window);
    m_timeSlotLabel.draw(window);

    m_specializationInput.draw(window);
    m_doctorIdInput.draw(window);
    m_dateInput.draw(window);
    m_timeSlotInput.draw(window);

    m_submitButton.draw(window);
    m_backButton.draw(window);

    m_messagePanel.draw(window);

    renderMatchingDoctors(window);
}

bool BookAppointmentScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState BookAppointmentScreen::getRequestedState() const
{
    return m_requestedState;
}

void BookAppointmentScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}


bool BookAppointmentScreen::hasDoctorsForSpecialization(const char* specialization) const
{
    const Doctor* doctorData = m_backend.getDoctors().getAll();

    for (int i = 0; i < m_backend.getDoctors().size(); i++)
    {
        if (doctorData[i].getSpecialization().equalsIgnoreCase(specialization))
        {
            return true;
        }
    }

    return false;
}

void BookAppointmentScreen::renderMatchingDoctors(sf::RenderWindow& window) const
{
    const CustomString& specialization = m_specializationInput.getText();
    if (specialization.isEmpty())
    {
        return;
    }

    const Doctor* doctorData = m_backend.getDoctors().getAll();

    sf::Text header;
    header.setFont(m_font);
    header.setCharacterSize(18);
    header.setFillColor(sf::Color::White);
    header.setPosition(540.f, 120.f);
    header.setString("Matching Doctors:");
    window.draw(header);

    float y = 155.f;
    bool found = false;

    for (int i = 0; i < m_backend.getDoctors().size(); i++)
    {
        if (doctorData[i].getSpecialization().equalsIgnoreCase(specialization.cStr()))
        {
            sf::Text row;
            row.setFont(m_font);
            row.setCharacterSize(16);
            row.setFillColor(sf::Color::White);
            row.setPosition(540.f, y);

            CustomString line("ID: ");
            line += doctorData[i].getId();
            line += " | ";
            line += doctorData[i].getName().cStr();
            line += " | Fee: PKR ";
            line += doctorData[i].getFee();

            row.setString(line.cStr());
            window.draw(row);

            y += 28.f;
            found = true;
        }
    }

    if (!found)
    {
        sf::Text none;
        none.setFont(m_font);
        none.setCharacterSize(16);
        none.setFillColor(sf::Color::White);
        none.setPosition(540.f, 155.f);
        none.setString("No doctors available for that specialization.");
        window.draw(none);
    }
}