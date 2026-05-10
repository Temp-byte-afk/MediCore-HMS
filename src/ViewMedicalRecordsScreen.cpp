#include "ViewMedicalRecordsScreen.h"
#include "DateUtils.h"
#include <iostream>

ViewMedicalRecordsScreen::ViewMedicalRecordsScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_backButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::ViewMedicalRecords;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "My Medical Records", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Sorted by date descending", 20, sf::Vector2f(70.f, 80.f));
    m_patientLabel = Label(m_font, "Patient: ", 22, sf::Vector2f(70.f, 115.f));
    m_headerLabel = Label(m_font, "Date | Doctor Name | Medicines | Notes", 18, sf::Vector2f(70.f, 180.f));

    m_backButton = Button(m_font, "Back", sf::Vector2f(70.f, 640.f), sf::Vector2f(180.f, 45.f));
    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 565.f), sf::Vector2f(650.f, 55.f), 18);
}

void ViewMedicalRecordsScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

int ViewMedicalRecordsScreen::comparePrescriptionsForDisplay(const Prescription& first, const Prescription& second) const
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

void ViewMedicalRecordsScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_backButton.handleEvent(event, window);

    if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::PatientDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::PatientDashboard);
    }
}

void ViewMedicalRecordsScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
}

void ViewMedicalRecordsScreen::renderPrescriptionList(sf::RenderWindow& window) const
{
    const int patientId = m_backend.getCurrentUserId();
    const Prescription* prescriptionData = m_backend.getPrescriptions().getAll();
    const Doctor* doctorData = m_backend.getDoctors().getAll();

    int count = 0;
    for (int i = 0; i < m_backend.getPrescriptions().size(); i++)
    {
        if (prescriptionData[i].getPatientId() == patientId)
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
        none.setString("No medical records found.");
        window.draw(none);
        return;
    }

    const Prescription** sortedPrescriptions = new const Prescription * [count];
    int index = 0;

    for (int i = 0; i < m_backend.getPrescriptions().size(); i++)
    {
        if (prescriptionData[i].getPatientId() == patientId)
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

    float y = 220.f;
    int visibleCount = 0;

    for (int i = 0; i < count; i++)
    {
        const Prescription& prescription = *sortedPrescriptions[i];
        const Doctor* doctor = nullptr;

        for (int j = 0; j < m_backend.getDoctors().size(); j++)
        {
            if (doctorData[j].getId() == prescription.getDoctorId())
            {
                doctor = &doctorData[j];
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

        line += prescription.getDate().cStr();
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
        line += prescription.getMedicines().cStr();
        line += " | ";
        line += prescription.getNotes().cStr();

        row.setString(line.cStr());
        window.draw(row);

        y += 40.f;
        visibleCount++;

        if (visibleCount >= 10)
        {
            break;
        }
    }

    delete[] sortedPrescriptions;
}

void ViewMedicalRecordsScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);

    CustomString patientText("Patient: ");
    patientText += m_backend.getCurrentUserName();
    m_patientLabel.setText(patientText.cStr());
    m_patientLabel.draw(window);

    m_headerLabel.draw(window);

    renderPrescriptionList(window);

    m_backButton.draw(window);
    m_messagePanel.draw(window);
}

bool ViewMedicalRecordsScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState ViewMedicalRecordsScreen::getRequestedState() const
{
    return m_requestedState;
}

void ViewMedicalRecordsScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}