#include "DoctorDashboardScreen.h"
#include <iostream>

DoctorDashboardScreen::DoctorDashboardScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_todayButton(),
    m_completeButton(),
    m_noShowButton(),
    m_prescriptionButton(),
    m_historyButton(),
    m_logoutButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::DoctorDashboard;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Doctor Dashboard", 34, sf::Vector2f(70.f, 30.f));
    m_welcomeLabel = Label(m_font, "Welcome, Doctor", 24, sf::Vector2f(70.f, 90.f));
    m_specializationLabel = Label(m_font, "Specialization: ", 22, sf::Vector2f(70.f, 130.f));

    m_todayButton = Button(m_font, "View Today's Appointments", sf::Vector2f(70.f, 200.f), sf::Vector2f(300.f, 45.f));
    m_completeButton = Button(m_font, "Mark Appointment Complete", sf::Vector2f(70.f, 260.f), sf::Vector2f(300.f, 45.f));
    m_noShowButton = Button(m_font, "Mark Appointment No-Show", sf::Vector2f(70.f, 320.f), sf::Vector2f(300.f, 45.f));
    m_prescriptionButton = Button(m_font, "Write Prescription", sf::Vector2f(70.f, 380.f), sf::Vector2f(300.f, 45.f));
    m_historyButton = Button(m_font, "View Patient History", sf::Vector2f(70.f, 440.f), sf::Vector2f(300.f, 45.f));
    m_logoutButton = Button(m_font, "Logout", sf::Vector2f(70.f, 520.f), sf::Vector2f(300.f, 45.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(420.f, 220.f), sf::Vector2f(720.f, 90.f), 20);
}

void DoctorDashboardScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void DoctorDashboardScreen::showComingSoon(const char* message)
{
    m_messagePanel.setMessage(message);
}

void DoctorDashboardScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_todayButton.handleEvent(event, window);
    m_completeButton.handleEvent(event, window);
    m_noShowButton.handleEvent(event, window);
    m_prescriptionButton.handleEvent(event, window);
    m_historyButton.handleEvent(event, window);
    m_logoutButton.handleEvent(event, window);

    if (m_todayButton.isClicked(event, window))
    {
        requestScreen(ScreenState::ViewTodayAppointments);
    }
    else if (m_completeButton.isClicked(event, window))
    {
        requestScreen(ScreenState::MarkComplete);
    }
    else if (m_noShowButton.isClicked(event, window))
    {
        requestScreen(ScreenState::MarkNoShow);
    }
    else if (m_prescriptionButton.isClicked(event, window))
    {
        requestScreen(ScreenState::WritePrescription);
    }
    else if (m_historyButton.isClicked(event, window))
    {
        requestScreen(ScreenState::DoctorHistory);
    }
    else if (m_logoutButton.isClicked(event, window))
    {
        m_backend.logout();
        requestScreen(ScreenState::Login);
    }
}

void DoctorDashboardScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
}

void DoctorDashboardScreen::render(sf::RenderWindow& window)
{
    refreshDoctorInfo();

    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_welcomeLabel.draw(window);
    m_specializationLabel.draw(window);

    m_todayButton.draw(window);
    m_completeButton.draw(window);
    m_noShowButton.draw(window);
    m_prescriptionButton.draw(window);
    m_historyButton.draw(window);
    m_logoutButton.draw(window);

    m_messagePanel.draw(window);
}

bool DoctorDashboardScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState DoctorDashboardScreen::getRequestedState() const
{
    return m_requestedState;
}

void DoctorDashboardScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}

void DoctorDashboardScreen::refreshDoctorInfo()
{
    const Doctor* doctor = m_backend.getDoctors().findById(m_backend.getCurrentUserId());

    if (doctor == nullptr)
    {
        m_welcomeLabel.setText("Welcome, Doctor");
        m_specializationLabel.setText("Specialization: ");
        return;
    }

    CustomString welcomeText("Welcome, Dr. ");
    welcomeText += doctor->getName();
    m_welcomeLabel.setText(welcomeText.cStr());

    CustomString specText("Specialization: ");
    specText += doctor->getSpecialization();
    m_specializationLabel.setText(specText.cStr());
}

void DoctorDashboardScreen::onEnter()
{
    refreshDoctorInfo();
}