#include "DoctorHistoryScreen.h"
#include <iostream>
#include "DateUtils.h"

DoctorHistoryScreen::DoctorHistoryScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_patientIdInput()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::DoctorHistory;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "View Patient Medical History", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "You can only view your own patients.", 20, sf::Vector2f(70.f, 80.f));
    m_doctorLabel = Label(m_font, "Doctor: ", 22, sf::Vector2f(70.f, 115.f));
    m_patientIdLabel = Label(m_font, "Patient ID", 22, sf::Vector2f(70.f, 200.f));
    m_headerLabel = Label(m_font, "Date | Medicines | Notes", 18, sf::Vector2f(70.f, 350.f));
    m_accessLabel = Label(m_font, "Access check", 20, sf::Vector2f(70.f, 310.f));

    m_patientIdInput = InputBox(
        m_font,
        "Enter patient ID",
        sf::Vector2f(70.f, 235.f),
        sf::Vector2f(380.f, 45.f),
        20,
        false,
        20
    );

    m_submitButton = Button(m_font, "View History", sf::Vector2f(70.f, 315.f), sf::Vector2f(180.f, 50.f));
    m_backButton = Button(m_font, "Back", sf::Vector2f(270.f, 315.f), sf::Vector2f(180.f, 50.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 580.f), sf::Vector2f(620.f, 70.f), 18);
}

void DoctorHistoryScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

bool DoctorHistoryScreen::doctorHasAccessToPatient(int doctorId, int patientId) const
{
    const Appointment* appointments = m_backend.getAppointments().getAll();

    for (int i = 0; i < m_backend.getAppointments().size(); i++)
    {
        if (appointments[i].getDoctorId() == doctorId &&
            appointments[i].getPatientId() == patientId &&
            appointments[i].isCompleted())
        {
            return true;
        }
    }

    return false;
}

int DoctorHistoryScreen::comparePrescriptionsForDisplay(const Prescription& first, const Prescription& second) const
{
    int dateCompare = DateUtils::compareDates(first.getDate().cStr(), second.getDate().cStr());

    if (dateCompare != 0)
    {
        return -dateCompare;
    }

    if (first.getPrescriptionId() > second.getPrescriptionId())
    {
        return -1;
    }
    if (first.getPrescriptionId() < second.getPrescriptionId())
    {
        return 1;
    }

    return 0;
}

void DoctorHistoryScreen::submitLookup()
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

    int doctorId = m_backend.getCurrentUserId();

    if (!doctorHasAccessToPatient(doctorId, patientId))
    {
        m_messagePanel.setMessage("Access denied. You can only view records of your own patients.");
        return;
    }

    m_messagePanel.setMessage("Patient history loaded.");
    m_accessLabel.setText("Access granted.");
}

void DoctorHistoryScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_patientIdInput.handleEvent(event, window);
    m_submitButton.handleEvent(event, window);
    m_backButton.handleEvent(event, window);

    if (m_submitButton.isClicked(event, window))
    {
        submitLookup();
    }
    else if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::DoctorDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        submitLookup();
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::DoctorDashboard);
    }
}

void DoctorHistoryScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
    m_patientIdInput.update();
}

void DoctorHistoryScreen::renderPrescriptionList(sf::RenderWindow& window, int patientId) const
{
    const int doctorId = m_backend.getCurrentUserId();
    const Prescription* prescriptionData = m_backend.getPrescriptions().getAll();

    int count = 0;
    for (int i = 0; i < m_backend.getPrescriptions().size(); i++)
    {
        if (prescriptionData[i].getDoctorId() == doctorId &&
            prescriptionData[i].getPatientId() == patientId)
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
        none.setPosition(70.f, 390.f);
        none.setString("No medical records found.");
        window.draw(none);
        return;
    }

    const Prescription** sortedPrescriptions = new const Prescription * [count];
    int index = 0;

    for (int i = 0; i < m_backend.getPrescriptions().size(); i++)
    {
        if (prescriptionData[i].getDoctorId() == doctorId &&
            prescriptionData[i].getPatientId() == patientId)
        {
            sortedPrescriptions[index] = &prescriptionData[i];
            index++;
        }
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (comparePrescriptionsForDisplay(*sortedPrescriptions[j], *sortedPrescriptions[j + 1]) > 0)
            {
                const Prescription* temp = sortedPrescriptions[j];
                sortedPrescriptions[j] = sortedPrescriptions[j + 1];
                sortedPrescriptions[j + 1] = temp;
            }
        }
    }

    float y = 390.f;
    int visibleCount = 0;

    for (int i = 0; i < count; i++)
    {
        const Prescription& prescription = *sortedPrescriptions[i];

        sf::Text row;
        row.setFont(m_font);
        row.setCharacterSize(16);
        row.setFillColor(sf::Color::White);
        row.setPosition(70.f, y);

        CustomString line;
        line.setText("");
        line += prescription.getDate().cStr();
        line += " | ";
        line += prescription.getMedicines().cStr();
        line += " | ";
        line += prescription.getNotes().cStr();

        row.setString(line.cStr());
        window.draw(row);

        y += 26.f;
        visibleCount++;

        if (visibleCount >= 8)
        {
            break;
        }
    }

    delete[] sortedPrescriptions;
}

void DoctorHistoryScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);

    CustomString doctorText("Doctor: ");
    doctorText += m_backend.getCurrentUserName();
    m_doctorLabel.setText(doctorText.cStr());
    m_doctorLabel.draw(window);

    m_patientIdLabel.draw(window);
    m_patientIdInput.draw(window);

    m_submitButton.draw(window);
    m_backButton.draw(window);

    m_accessLabel.draw(window);

    m_headerLabel.draw(window);

    const CustomString& inputText = m_patientIdInput.getText();
    if (!inputText.isEmpty() && Validator::isValidId(inputText.cStr()))
    {
        int patientId = 0;
        const char* idChars = inputText.cStr();
        int i = 0;
        while (idChars[i] != '\0')
        {
            patientId = patientId * 10 + (idChars[i] - '0');
            i++;
        }

        if (doctorHasAccessToPatient(m_backend.getCurrentUserId(), patientId))
        {
            renderPrescriptionList(window, patientId);
        }
    }

    m_messagePanel.draw(window);
}

bool DoctorHistoryScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState DoctorHistoryScreen::getRequestedState() const
{
    return m_requestedState;
}

void DoctorHistoryScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}