#include "AdminDashboardScreen.h"
#include <iostream>

AdminDashboardScreen::AdminDashboardScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_addDoctorButton(),
    m_removeDoctorButton(),
    m_viewPatientsButton(),
    m_viewDoctorsButton(),
    m_viewAppointmentsButton(),
    m_viewUnpaidBillsButton(),
    m_dischargeButton(),
    m_securityLogButton(),
    m_dailyReportButton(),
    m_logoutButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::AdminDashboard;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Admin Panel - MediCore", 34, sf::Vector2f(70.f, 30.f));
    m_welcomeLabel = Label(m_font, "Welcome, Admin", 24, sf::Vector2f(70.f, 90.f));

    m_addDoctorButton = Button(m_font, "Add Doctor", sf::Vector2f(70.f, 170.f), sf::Vector2f(270.f, 42.f));
    m_removeDoctorButton = Button(m_font, "Remove Doctor", sf::Vector2f(70.f, 220.f), sf::Vector2f(270.f, 42.f));
    m_viewPatientsButton = Button(m_font, "View All Patients", sf::Vector2f(70.f, 270.f), sf::Vector2f(270.f, 42.f));
    m_viewDoctorsButton = Button(m_font, "View All Doctors", sf::Vector2f(70.f, 320.f), sf::Vector2f(270.f, 42.f));
    m_viewAppointmentsButton = Button(m_font, "View All Appointments", sf::Vector2f(70.f, 370.f), sf::Vector2f(270.f, 42.f));
    m_viewUnpaidBillsButton = Button(m_font, "View Unpaid Bills", sf::Vector2f(70.f, 420.f), sf::Vector2f(270.f, 42.f));
    m_dischargeButton = Button(m_font, "Discharge Patient", sf::Vector2f(70.f, 470.f), sf::Vector2f(270.f, 42.f));
    m_securityLogButton = Button(m_font, "View Security Log", sf::Vector2f(70.f, 520.f), sf::Vector2f(270.f, 42.f));
    m_dailyReportButton = Button(m_font, "Generate Daily Report", sf::Vector2f(70.f, 570.f), sf::Vector2f(270.f, 42.f));
    m_logoutButton = Button(m_font, "Logout", sf::Vector2f(70.f, 650.f), sf::Vector2f(270.f, 42.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(400.f, 220.f), sf::Vector2f(760.f, 90.f), 20);
}

void AdminDashboardScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void AdminDashboardScreen::showComingSoon(const char* message)
{
    m_messagePanel.setMessage(message);
}

void AdminDashboardScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_addDoctorButton.handleEvent(event, window);
    m_removeDoctorButton.handleEvent(event, window);
    m_viewPatientsButton.handleEvent(event, window);
    m_viewDoctorsButton.handleEvent(event, window);
    m_viewAppointmentsButton.handleEvent(event, window);
    m_viewUnpaidBillsButton.handleEvent(event, window);
    m_dischargeButton.handleEvent(event, window);
    m_securityLogButton.handleEvent(event, window);
    m_dailyReportButton.handleEvent(event, window);
    m_logoutButton.handleEvent(event, window);

    if (m_addDoctorButton.isClicked(event, window))
    {
        requestScreen(ScreenState::AddDoctor);
    }
    else if (m_removeDoctorButton.isClicked(event, window))
    {
        requestScreen(ScreenState::RemoveDoctor);
    }
    else if (m_viewPatientsButton.isClicked(event, window))
    {
        requestScreen(ScreenState::ViewAllPatients);
    }
    else if (m_viewDoctorsButton.isClicked(event, window))
    {
        requestScreen(ScreenState::ViewAllDoctors);
    }
    else if (m_viewAppointmentsButton.isClicked(event, window))
    {
        requestScreen(ScreenState::ViewAllAppointments);
    }
    else if (m_viewUnpaidBillsButton.isClicked(event, window))
    {
        requestScreen(ScreenState::ViewUnpaidBills);
    }
    else if (m_dischargeButton.isClicked(event, window))
    {
        requestScreen(ScreenState::DischargePatient);
    }
    else if (m_securityLogButton.isClicked(event, window))
    {
        requestScreen(ScreenState::SecurityLog);
    }
    else if (m_dailyReportButton.isClicked(event, window))
    {
        requestScreen(ScreenState::DailyReport);
    }
    else if (m_logoutButton.isClicked(event, window))
    {
        m_backend.logout();
        requestScreen(ScreenState::Login);
    }
}

void AdminDashboardScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
}

void AdminDashboardScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_welcomeLabel.draw(window);

    m_addDoctorButton.draw(window);
    m_removeDoctorButton.draw(window);
    m_viewPatientsButton.draw(window);
    m_viewDoctorsButton.draw(window);
    m_viewAppointmentsButton.draw(window);
    m_viewUnpaidBillsButton.draw(window);
    m_dischargeButton.draw(window);
    m_securityLogButton.draw(window);
    m_dailyReportButton.draw(window);
    m_logoutButton.draw(window);

    m_messagePanel.draw(window);
}

bool AdminDashboardScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState AdminDashboardScreen::getRequestedState() const
{
    return m_requestedState;
}

void AdminDashboardScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}