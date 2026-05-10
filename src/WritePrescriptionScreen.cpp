#include "WritePrescriptionScreen.h"
#include <iostream>

WritePrescriptionScreen::WritePrescriptionScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_appointmentIdInput(),
    m_medicinesInput(),
    m_notesInput()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::WritePrescription;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Write Prescription", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Only completed appointments can receive prescriptions.", 20, sf::Vector2f(70.f, 80.f));
    m_doctorLabel = Label(m_font, "Doctor: ", 22, sf::Vector2f(70.f, 115.f));

    m_appointmentIdLabel = Label(m_font, "Appointment ID", 22, sf::Vector2f(70.f, 180.f));
    m_medicinesLabel = Label(m_font, "Medicines", 22, sf::Vector2f(70.f, 270.f));
    m_notesLabel = Label(m_font, "Notes", 22, sf::Vector2f(70.f, 360.f));
    m_completedListLabel = Label(m_font, "Your Completed Appointments", 24, sf::Vector2f(650.f, 30.f));

    m_appointmentIdInput = InputBox(
        m_font,
        "Enter appointment ID",
        sf::Vector2f(70.f, 215.f),
        sf::Vector2f(500.f, 45.f),
        20,
        false,
        20
    );

    m_medicinesInput = InputBox(
        m_font,
        "e.g. Paracetamol 500mg;Amoxicillin 250mg",
        sf::Vector2f(70.f, 305.f),
        sf::Vector2f(500.f, 45.f),
        20,
        false,
        500
    );

    m_notesInput = InputBox(
        m_font,
        "Enter notes",
        sf::Vector2f(70.f, 395.f),
        sf::Vector2f(500.f, 45.f),
        20,
        false,
        300
    );

    m_submitButton = Button(m_font, "Save Prescription", sf::Vector2f(70.f, 475.f), sf::Vector2f(200.f, 50.f));
    m_backButton = Button(m_font, "Back", sf::Vector2f(290.f, 475.f), sf::Vector2f(180.f, 50.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 550.f), sf::Vector2f(560.f, 90.f), 18);
}

void WritePrescriptionScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void WritePrescriptionScreen::submitPrescription()
{
    const CustomString& appointmentIdText = m_appointmentIdInput.getText();
    const CustomString& medicinesText = m_medicinesInput.getText();
    const CustomString& notesText = m_notesInput.getText();

    if (appointmentIdText.isEmpty() || medicinesText.isEmpty() || notesText.isEmpty())
    {
        m_messagePanel.setMessage("Please fill all fields.");
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

    int doctorId = m_backend.getCurrentUserId();

    try
    {
        m_backend.writePrescription(
            doctorId,
            appointmentId,
            medicinesText.cStr(),
            notesText.cStr()
        );

        m_messagePanel.setMessage("Prescription saved.");
    }
    catch (const HospitalException& ex)
    {
        m_messagePanel.setMessage(ex.what());
    }
}

void WritePrescriptionScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_appointmentIdInput.handleEvent(event, window);
    m_medicinesInput.handleEvent(event, window);
    m_notesInput.handleEvent(event, window);

    m_submitButton.handleEvent(event, window);
    m_backButton.handleEvent(event, window);

    if (m_submitButton.isClicked(event, window))
    {
        submitPrescription();
    }
    else if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::DoctorDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        submitPrescription();
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::DoctorDashboard);
    }
}

void WritePrescriptionScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
    m_appointmentIdInput.update();
    m_medicinesInput.update();
    m_notesInput.update();
}

void WritePrescriptionScreen::renderCompletedAppointments(sf::RenderWindow& window) const
{
    int doctorId = m_backend.getCurrentUserId();
    const Appointment* appointmentData = m_backend.getAppointments().getAll();
    const Patient* patientData = m_backend.getPatients().getAll();

    bool found = false;
    float y = 70.f;

    sf::Text header;
    header.setFont(m_font);
    header.setCharacterSize(18);
    header.setFillColor(sf::Color::White);
    header.setPosition(650.f, 80.f);
    header.setString("Appointment ID | Patient Name | Date | Time");
    window.draw(header);

    y += 28.f;

    for (int i = 0; i < m_backend.getAppointments().size(); i++)
    {
        const Appointment& appointment = appointmentData[i];

        if (appointment.getDoctorId() == doctorId && appointment.isCompleted())
        {
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
            row.setCharacterSize(16);
            row.setFillColor(sf::Color::White);
            row.setPosition(650.f, y);

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
            line += appointment.getDate().cStr();
            line += " | ";
            line += appointment.getTimeSlot().cStr();

            row.setString(line.cStr());
            window.draw(row);

            y += 26.f;
            found = true;

            if (y > 620.f)
            {
                break;
            }
        }
    }

    if (!found)
    {
        sf::Text none;
        none.setFont(m_font);
        none.setCharacterSize(16);
        none.setFillColor(sf::Color::White);
        none.setPosition(650.f, 120.f);
        none.setString("No completed appointments found.");
        window.draw(none);
    }
}

void WritePrescriptionScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);

    CustomString doctorText("Doctor: ");
    doctorText += m_backend.getCurrentUserName();
    m_doctorLabel.setText(doctorText.cStr());
    m_doctorLabel.draw(window);

    m_appointmentIdLabel.draw(window);
    m_medicinesLabel.draw(window);
    m_notesLabel.draw(window);
    m_completedListLabel.draw(window);

    m_appointmentIdInput.draw(window);
    m_medicinesInput.draw(window);
    m_notesInput.draw(window);

    m_submitButton.draw(window);
    m_backButton.draw(window);

    m_messagePanel.draw(window);

    renderCompletedAppointments(window);
}

bool WritePrescriptionScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState WritePrescriptionScreen::getRequestedState() const
{
    return m_requestedState;
}

void WritePrescriptionScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}